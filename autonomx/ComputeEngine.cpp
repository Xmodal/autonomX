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
#include "GOLPatternType.h"

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

void ComputeEngine::receiveOscData(int generatorId, QVariantList data) {
    if(!generatorsHashMap->contains(generatorId)) {
        throw std::runtime_error("generator does not exist");
    }
    QSharedPointer<Generator> generator = generatorsHashMap->value(generatorId);

    QList<QVariant> dataAsList = data;
    int argumentsTotal = data.size();
    int argumentsValid = 0;

    for(int i = 0; i < generator->getInputRegionSet()->rowCount(); i++) {
        // set default to 0
        double input = 0;
        // check that the message's list is long enough
        if(i < data.size()) {
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

    // alerts loop that new value was received via inputOSC and can be reflected in lattice
    inputValueReceived = true;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );
        qDebug() << "receiveOscData (ComputeEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\tdata = " << data << "\t(" << argumentsValid << " of " << argumentsTotal << " valid)";
    }
}

void ComputeEngine::receiveOscGeneratorControlMessage(int generatorId, QVariantList data, QString controlMessage) {
    QString generatorName, inputType, parameter1, parameter2;
    QList<QVariant> dataAsList = data;
    int inputValue = 0;

    // check if generator exists -> if not, ignore
    if(!generatorsHashMap->contains(generatorId)) return;

    // split input osc control message into pieces
    QStringList controlMessageList = controlMessage.split(QLatin1Char('/'), Qt::SkipEmptyParts);
    if(controlMessageList.length() < 2) return;

    // assign split osc message pieces to corresponding variables
    for(int i = 0; i < controlMessageList.length(); i++) {
        switch(i) {
        case 0:
            generatorName = controlMessageList.at(0).toLower();
            break;
        case 1:
            inputType = controlMessageList.at(1).toLower();
            break;
        case 2:
            parameter1 = controlMessageList.at(2).toLower();
            break;
        case 3:
            parameter2 = controlMessageList.at(3).toLower();
            break;
        }
    }

    // if the first parameter is empty, message was entered incorrectly
    if(parameter1 == "") {
        qDebug() << "empty control message entered!";
        return;
    }

    // check if targetted generator name exists -> else, ignore
    QSharedPointer<Generator> generator = generatorsHashMap->value(generatorId);
    if(!(generatorName == generator->getGeneratorName().toLower())) {
        // name not found -> skip and return
        return;
    }

    // verify dataList isn't empty
    if(!dataAsList.isEmpty()) {
        if(dataAsList.length() == 1) {
            inputValue = dataAsList.at(0).toInt();
        }
    }

    if(parameter1.contains("_") || parameter2.contains("_")) {
        parameter1.remove(QChar('_'));
        parameter2.remove(QChar('_'));
    }

    // if user-input control message better-matched our QProperty system formatting -> many types of messages could be handled here
//    if(parameterControlList.contains(parameter1)) {
//        if(inputValue < 1) inputValue = 1;

//        QByteArray controlMessageArray = parameter1.toLocal8Bit();
//        const char *controlMessageArrayChar = controlMessageArray.data();
//        generator->setProperty(controlMessageArrayChar, inputValue);
//        return;
//    }

    // check if control message is global or generator-specific
    if(parameterControlList.value(parameter1) == "global") {
        if(inputValue < 1) inputValue = 1;

        // control message is global parameter
        if(parameter1 == "width") {
            generator->writeLatticeWidth(inputValue);
        } else if(parameter1 == "height") {
            generator->writeLatticeHeight(inputValue);
        } else if(parameter1 == "timescale") {
            generator->writeTimeScale(inputValue);
        } else if(parameter1 == "restart") {
            generator->initialize();
        } else if(parameter1 == "reset") {
            generator->resetParameters();
        } else if(parameter1 == "reset_regions") {
            generator->resetRegions();
        }
    }
    // control message is not global param
    else {

        // TODO: codify control messages with internal naming system
        // -> this will maximize advantage of QProperty system
        // -> can condense code below to a fraction of the size

        // now handles control message separately dependending on the type of generator
        if(generator->getType() == "SpikingNet") {

            if(parameter1.contains("neurontype")) {
                QString neuronType = parameter2;
                NeuronType neuron;

                // search for / assign neuron type
                if(neuronType == "spiking") {
                    neuron = NeuronType::SpikingNeuron;
                } else if(neuronType == "spikingrand") {
                    neuron = NeuronType::SpikingNeuronRandomized;
                } else if(neuronType == "resonator") {
                    neuron = NeuronType::ResonatorNeuron;
                } else if(neuronType == "resonatorrand") {
                    neuron = NeuronType::ResonatorNeuronRandomized;
                } else if(neuronType == "chattering") {
                    neuron = NeuronType::ChatteringNeuron;
                } else {
                    qDebug() << "WARNING: invalid neuron type";
                    return;
                }

                if(parameter1 == "excitatoryneurontype") {
                    generator->setProperty("excitatoryNeuronType", neuron);
                } else if(parameter1 == "inhibitoryneurontype") {
                    generator->setProperty("inhibitoryNeuronType", neuron);
                } else {
                    qDebug() << "WARNING: invalid neuron meta type";
                }
            } else if(parameter1 == "stpstrength") {
                parameter1 = "STPStrength";
            } else if(parameter1 == "stpdstrength") {
                parameter1 = "STDPStrength";
            } else if(parameter1 == "decayhalflife") {
                parameter1 = "decayHalfLife";
            }

            // indicates a learning control message
            if(parameter2 == "on" || parameter2 == "off") {
                bool toggleDirection = false;
                parameter1.prepend("flag_");
                if(parameter2 == "on") toggleDirection = true;

                QByteArray neuronMessageArray = parameter1.toLocal8Bit();
                const char *neuronMessageChar = neuronMessageArray.data();

                // execute learning control message
                generator->setProperty(neuronMessageChar, toggleDirection);
                return;
            }

            if(parameter1 == "inhibitoryneuronnoise") {
                parameter1 = "inhibitoryNoise";
            } else if(parameter1 == "excitatoryneuronnoise") {
                parameter1 = "excitatoryNoise";
            } else if(parameter1 == "inhibitoryportion") {
                parameter1 = "inhibitoryPortion";
            }

            // message has now been sorted and formatted
            // make changes uses QProperty system
            QByteArray controlMessageArray = parameter1.toLocal8Bit();
            const char *controlMessageArrayChar = controlMessageArray.data();
            generator->setProperty(controlMessageArrayChar, inputValue);

        } else if(generator->getType() == "GameOfLife") {
            GOLPatternType golPattern;

            if(parameter1 == "golpatterntype") {
                if(parameter2 == "random") {
                    golPattern = GOLPatternType::Random;
                } else if(parameter2 == "glider") {
                    golPattern = GOLPatternType::Glider;
                } else if(parameter2 == "spaceship") {
                    golPattern = GOLPatternType::SpaceShip;
                } else if(parameter2 == "rpentomino") {
                    golPattern = GOLPatternType::RPentoMino;
                } else if(parameter2 == "pentadecathlon") {
                    golPattern = GOLPatternType::Pentadecathlon;
                } else {
                    qDebug() << "WARNING: invalid GameOfLife pattern type";
                    return;
                }
            }

            // TODO: FIX GOLPattern QProperty
            // -> only accepts QString "GOLPattern"
            // -> should generically accept char array as commented out below ->
//                QByteArray patternTypeArray = parameter1.toLocal8Bit();
//                const char *patternTypeArrayChar = patternTypeArray.data();
//                generator->setProperty(patternTypeArrayChar, golPattern);

            // set GOL pattern type
            generator->setProperty("GOLPattern", golPattern);

        } else if(generator->getType() == "WolframCA") {

            // check if message targets random_seed
            if(parameter1 == "randomseed") {
                parameter1 = "flag_randSeed";

                if(parameter2 == "on" || parameter2 == "off") {
                    bool toggleDirection = false;
                    if(parameter2 == "on") toggleDirection = true;

                    QByteArray randSeedArray = parameter1.toLocal8Bit();
                    const char *randSeedArrayChar = randSeedArray.data();
                    // execute randomSeed toggle
                    generator->setProperty(randSeedArrayChar, toggleDirection);
                }
            }

            // otherwise must be targetting rule number
            else {
                // TODO: FIX WolframCA rule QProperty
                // -> only accepts QString "rule"
                // but should generically accept char array as commented out below ->
//                QByteArray ruleArray = parameter1.toLocal8Bit();
//                const char *ruleArrayChar = ruleArray.data();
//                generator->setProperty(ruleArrayChar, inputValue);
                generator->setProperty("rule", inputValue);
            }
        }
        // if not any pre-built generator type or global parameter
        // param change will only execute if osc control message matches the associated QProperty *exactly*
        // -> see documentation for more details
        else {
            int input = dataAsList.at(0).toInt();
            double inputDouble = input;

            QByteArray controlMessageArray = controlMessage.toLocal8Bit();
            const char *controlMessageChar = controlMessageArray.data();

            // execute control message of any generic format that was not for pre-built generators
            generator->setProperty(controlMessageChar, inputDouble);
        }
    }
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

    // check if input value received via OSC this loop
    if(inputValueReceived) {
        // apply input values
        for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
            (*it)->applyInputRegion();
        }

        // reset check to prevent values being erased on every loop
        inputValueReceived = false;
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
        parameterControlList["time_scale"] = "global";
        parameterControlList["restart"] = "global";
        parameterControlList["reset"] = "global";
        parameterControlList["reset_regions"] = "global";

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
