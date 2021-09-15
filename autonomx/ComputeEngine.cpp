// Copyright 2020, Xmodal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <algorithm>
#include <chrono>
#include <QDebug>
#include <QThread>

#include "ComputeEngine.h"
#include "AppModel.h"
#include "NeuronType.h"
#include "PatternType.h"

ComputeEngine::ComputeEngine(QSharedPointer<QList<QSharedPointer<Generator>>> generatorsList, QSharedPointer<QHash<int, QSharedPointer<Generator>>> generatorsHashMap) : randomUniform(0.0, 1.0) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (ComputeEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->generatorsList = generatorsList;
    this->generatorsHashMap = generatorsHashMap;
}

ComputeEngine::~ComputeEngine() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );// get generator

        qDebug() << "destructor (ComputeEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
}

void ComputeEngine::receiveOscData(int generatorId, QVariantList data, QString generatorAddress) {
    if(!generatorsHashMap->contains(generatorId)) {
        throw std::runtime_error("generator does not exist");
    }

    QSharedPointer<Generator> generator = generatorsHashMap->value(generatorId);
    QList<QVariant> dataAsList = data;
    int argumentsTotal = data.size();
    int argumentsValid = 0;
    int generatorRegion;
    double input = 0;

    // check if message is intended for this generator -> if not, skip
    QStringList generatorAddressList = generatorAddress.split(QLatin1Char('/'));
    QString generatorName = generatorAddressList.at(1);
    if(generator->getGeneratorName() != generatorName) {
        return;
    }

    // check to see if input message is for specific generator region or all regions
    if(generatorAddressList.length() > 3) {
        generatorRegion = generatorAddressList.at(3).toInt();

        if(argumentsTotal > 0) {
            input = dataAsList.at(0).toDouble();

            // ensure generator region isn't out of range
            if(generator->getInputRegionSet()->rowCount() >= generatorRegion && generatorRegion > 0) {

                // if generator region is valid -> execute input message on specified input region
                generator->getInputRegionSet()->at(generatorRegion - 1)->writeIntensity(input);
            } else {
                // if generator region is invalid -> ignore input and print error message
                qDebug() << "ERROR: targeted Generator Region " << generatorRegion << " is out of range or doesn't exist!";
                return;
            }
        }
    } else {

        // execute input message on all regions of this generator
        for(int i = 0; i < generator->getInputRegionSet()->rowCount(); i++) {
            // set default to 0
            input = 0;

            // check that the message's list is long enough
            if(data.size() > i) {
                // check the message's type can be cast to double
                QMetaType::Type type = (QMetaType::Type) dataAsList.at(i).type();
                if(type == QMetaType::Float || type == QMetaType::Double || type == QMetaType::Int || type == QMetaType::Long) {
                    input = dataAsList.at(i).toDouble();
                    argumentsValid++;
                }
            }
            // write to input
            generator->getInputRegionSet()->at(i)->writeIntensity(input);

        }
    }

    // alerts loop that new value was received via inputOSC and can be reflected in lattice
    inputValueReceived = true;

    // used to reset the input values of this generator once this input has taken effect
    generatorIdWritten = generatorId;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );
        qDebug() << "receiveOscData (ComputeEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\tdata = " << data << "\t(" << argumentsValid << " of " << argumentsTotal << " valid)";
    }
}

void ComputeEngine::receiveOscGeneratorControlMessage(int generatorId, QVariantList data, QString controlMessage) {
    QString generatorName, inputType, inputParameter1, inputParameter2;
    bool inputBool = false, booleanMessageCheck = false, enumMessageCheck = false;
    QList<QVariant> dataAsList = data;
    double inputValue = 0;
    QByteArray controlMessageArray;
    const char *controlMessageArrayChar;

    // check if generator exists -> if not, ignore
    if(!generatorsHashMap->contains(generatorId)) return;

    // split input osc control message into list, items separated by placement of '/' in original message
    QStringList controlMessageList = controlMessage.split(QLatin1Char('/'));

    // remove any blank or empty strings in controlMessageList
    for(int i = 0; i < controlMessageList.length(); i++) {
        if (controlMessageList.at(i) == "" || controlMessageList.at(i).isEmpty()) {
            controlMessageList.removeAt(i);
        }
    }

    // assign input osc message components to corresponding variables
    for(int i = 0; i < controlMessageList.length(); i++) {
        switch(i) {
        case 0:
            generatorName = controlMessageList.at(0);
            break;
        case 1:
            inputType = controlMessageList.at(1);
            break;
        case 2:
            inputParameter1 = controlMessageList.at(2);
            // if parameter contains the word "Type" this indicates that the passed message is an enum value
            if(inputParameter1.contains("Type")) {
                enumMessageCheck = true;
            }
            break;
        case 3:
            inputParameter2 = controlMessageList.at(3);
            // if there is a second argument message this indicates that the passed message is turning on/off a parameter
            booleanMessageCheck = true;
            break;
        }
    }

    // check if targetted generator name exists -> else, will be ignored
    QSharedPointer<Generator> generator = generatorsHashMap->value(generatorId);
    QString inputGeneratorIdString = generatorName.right(2);
    QString generatorTypeCheck = generatorName;
    int inputID = inputGeneratorIdString.toInt();
    generatorTypeCheck.chop(3);

    if(generator->getID() != inputID) {
        if(generatorName != generator->getGeneratorName()) return;
    }

    // verify dataList isn't empty and then convert to double and assign for input
    if(!dataAsList.isEmpty()) {
        if(dataAsList.at(0).canConvert<double>()) {
            inputValue = dataAsList.at(0).toDouble();
        }
    }

    // verify that enum value entered is not out of range
    if(enumMessageCheck) {
        inputValue = inputValue - 1;
        // check GeneratorMeta::enumLabels Map for an enum type that corresponds to the osc message parameter
        if(generator->getMeta()->getEnumLabels().contains(parameterControlList.value(inputParameter1))) {
            // create a list of all the enum values are associated that that enum
            QList enumValues = generator->getMeta()->getEnumLabels().value(parameterControlList.value(inputParameter1)).toStringList();
            if(enumValues.length() < (inputValue + 1) || inputValue < 0) {
                // if value entered is out of range for this enum type -> error
                qDebug() << "ERROR: value entered is out of range (doesn't exist) for this parameter!";
                return;
            }
        }
    }

    ///// OSC External Control Message Execution Section /////

    // executes BOOLEAN control messages
    if(booleanMessageCheck) {
        inputParameter1.prepend("flag_");
        controlMessageArray = inputParameter1.toLocal8Bit();
        controlMessageArrayChar = controlMessageArray.data();
        if(inputParameter2.toLower() == "on") {
            inputBool = true;
        }
        generator->setProperty(controlMessageArrayChar, inputBool);
        return;
    }
        // control message is global parameter
        if(inputParameter1 == "width" || inputParameter1 == "height") {
            if(inputValue < 1) inputValue = 1;
            inputParameter1[0] = inputParameter1[0].toUpper();
            inputParameter1.prepend("lattice");
        } else if(inputParameter1 == "restart") {
            generator->initialize();
        } else if(inputParameter1 == "reset") {
            generator->resetParameters();
        }
        // TODO: this casuses a crash and can only be done once Ticket 375 (or related threading issue) has been resolved
        //        else if(parameter1 == "resetRegions") {
        //            generator->resetRegions();
        //        }

    // executes GENERATOR INPUT VALUE and ENUM control messages
    if(parameterControlList.contains(inputParameter1)) {
        controlMessageArray = inputParameter1.toLocal8Bit();
        controlMessageArrayChar = controlMessageArray.data();
        generator->setProperty(controlMessageArrayChar, inputValue);
        return;
    }

    ///////// Only Reaches here if message did not fit any acceptable OSC Input Message format ///////////
    qDebug() << "ERROR! -> Input Message was received as: " << controlMessage << " is not a valid OSC Control Message!";
    return;
}

void ComputeEngine::addGenerator(QSharedPointer<Generator> generator) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );// get generator

        qDebug() << "addGenerator (ComputeEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    generatorsList->append(generator);
    generatorsHashMap->insert(generator->getID(), generator);

    registerParameterControls(generator->getID());

}

void ComputeEngine::removeGenerator(QSharedPointer<Generator> generator) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );// get generator

        qDebug() << "removeGenerator (ComputeEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    generatorsList->removeOne(generator);
    generatorsHashMap->remove(generator->getID());
}

void ComputeEngine::start() {
    // this is called from an external thread initially and allows this object's thread to pick up the loop statement through its event queue
    // having a singleshot timer with time zero executes as soon as possible
    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    timer.singleShot(0, this, &ComputeEngine::loop);
}

void ComputeEngine::loop() {

    if(flagDisableProcessing) {
        return;
    }

    double millisCompute;   // time in nanoseconds taken by computation
    double millisLastFrame; // time in nanoseconds since last frame

    // compute time since last frame, with exception if this is the first frame
    if(firstFrame) {
        firstFrame = false;
        millisLastFrame = 1.0 / frequency * 1000.0;

    } else {
        millisLastFrame = elapsedTimer.nsecsElapsed() / 1000000.0;
    }

    // restart the timer to measure frame time and compute time
    elapsedTimer.restart();
    elapsedTimer.start();

    // check if input value received via OSC this loop or last loop
    if(inputValueReceived) {
        // apply input values
        for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
            (*it)->applyInputRegion();
        }

        // reset check to prevent values being erased on every loop
        inputValueReceived = false;
        // reset check to prevent values being rewritten on every loop
        generatorWrittenLastLoop = true;

    } else if(generatorWrittenLastLoop){
        QSharedPointer<Generator> generator = generatorsHashMap->value(generatorIdWritten);

        // reset all previously altered input regions back to 0
        for(int i = 0; i < generator->getInputRegionSet()->rowCount(); i++) {
            // write to input
            generator->getInputRegionSet()->at(i)->writeIntensity(0);
        }
        generator->applyInputRegion();
        generatorWrittenLastLoop = false;
    }

    // do the computation
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
        (*it)->computeIteration(1.0 / frequency);
    }

    // apply output values
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
        (*it)->applyOutputRegion();
    }

    // write to history
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
        double historyLatest = 0;
        for(int i = 0; i < (*it)->getOutputRegionSet()->rowCount(); i++) {
            historyLatest += (*it)->getOutputRegionSet()->at(i)->getIntensity();
        }
        if(flagDummyOutputMonitor) {
            // random output
            historyLatest = randomUniform(randomGenerator);
        } else {
            // dumb averaging
            historyLatest /= (*it)->getOutputRegionSet()->rowCount();
            // saturation polynomial
            historyLatest = (1.0 - pow(1.0 - historyLatest, 3));
        }
        // write value
        (*it)->writeHistoryLatest(historyLatest);
        // flip refresher to force update on QML side
        (*it)->flipHistoryRefresher();
    }

    // send output messages to osc engine
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
        QList<QVariant> oscOutputsList;
        QList<QVariant> oscOutputRegion;

        for(int i = 0; i < (*it)->getOutputRegionSet()->rowCount(); i++) {
            double value = flagDummyOutputMonitor ? randomUniform(randomGenerator) : (*it)->getOutputRegionSet()->at(i)->getIntensity();
            if(flagCastOutputToFloat) {
                oscOutputsList.append((float)value);
                oscOutputRegion.append((float)value);
            } else {
                oscOutputsList.append(value);
                oscOutputRegion.append(value);
            }

            // write and send individual output region osc messages
            // format is: "/[generator_name]/output/[output_region_number]/float"
            QString outputRegionNumber = QString::number(i+1);
            oscOutputRegion.prepend("/" + (*it)->getGeneratorName() + "/output/" + outputRegionNumber);
            emit sendOscData((*it)->getID(), oscOutputRegion);
            oscOutputRegion.clear();
        }

        // write generator info to osc output list message
        // format is: "/[generator_name]/output/float float float float" (or however many floats are needed to express every output region)
        oscOutputsList.prepend("/" + (*it)->getGeneratorName() + "/output");

        // send osc output list message
        emit sendOscData((*it)->getID(), oscOutputsList);
    }

    // measure the time used to do the computation
    millisCompute = elapsedTimer.nsecsElapsed() / 1000000.0;

    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    timer.singleShot((int) std::min<double>(1.0 / frequency * 1000.0, std::max<double>(1.0 / frequency * 1000.0 - millisCompute, 0)), this, &ComputeEngine::loop);

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "loop (ComputeEngine):\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\trefresh (ms) = " << millisLastFrame << "\tcompute (ms) = " << millisCompute;
    }
}

void ComputeEngine::registerParameterControls(int generatorId) {

    QSharedPointer<Generator> generator = generatorsHashMap->value(generatorId);
    GeneratorMeta* meta = generator->getMeta();

    // register global parameters if first pass
    if(firstPass) {
        parameterControlList["width"] = "global";
        parameterControlList["height"] = "global";
        parameterControlList["latticeWidth"] = "global";
        parameterControlList["latticeHeight"] = "global";
        parameterControlList["speed"] = "global";
        parameterControlList["restart"] = "global";
        parameterControlList["reset"] = "global";
        parameterControlList["resetRegions"] = "global";

        firstPass = false;
    }

     if(!registeredGeneratorTypes.contains(generator->getType())) {

//         addGeneratorType(generator->getType());

        QMapIterator<QString, QString> iter(meta->getGeneratorsParameterList());
        while(iter.hasNext()) {
            iter.next();
            parameterControlList[iter.key()] = iter.value();
        }

        QMapIterator<QString, QString> iterCheck(parameterControlList);
        while(iterCheck.hasNext()) {
            iterCheck.next();
        }
     }
}

//void ComputeEngine::addGeneratorType(QString generatorType) {
//    registeredGeneratorTypes.append(generatorType);
//    qDebug() << "ComputeEngine::addGeneratorTypes > registeredGeneratorTypes List: ";
//    for(int i = 0; i < registeredGeneratorTypes.size(); i++) {
//        qDebug() << registeredGeneratorTypes[i];
//    }
//}
