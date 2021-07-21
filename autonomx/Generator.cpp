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

#include <chrono>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

#include "Generator.h"
#include "AppModel.h"

Generator::Generator(int id, GeneratorMeta * meta) {
    this->id = id;
    this->meta = meta;
    // create default generatorName from generator type + id
    QString idString = QString::number(id).prepend("_0");
    this->generatorName = this->meta->getType() + idString;
//    QString idString = QString::number(id).prepend("0").append("_");
//    this->generatorName = idString + this->meta->getType();
    this->userNotes = "";

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (Generator)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id;
    }

    // create empty sets
    inputRegionSet = QSharedPointer<GeneratorRegionSet>(new GeneratorRegionSet(0));
    outputRegionSet = QSharedPointer<GeneratorRegionSet>(new GeneratorRegionSet(1));

    // connect row count signals
    // we need to do this as a proxy measure
    // because directly accessing a Qt property from GeneratorRegionSet throws an error
    QObject::connect(inputRegionSet.data(), &GeneratorRegionSet::rowCountChanged, this, [=](int inputCount) {
        emit valueChanged("inputCount", inputCount);
    });
    QObject::connect(outputRegionSet.data(), &GeneratorRegionSet::rowCountChanged, this, [=](int outputCount) {
        emit valueChanged("outputCount", outputCount);
    });
}


Generator::~Generator() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (Generator)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id;
    }
}

QString Generator::getName() {
    return meta->property("name").toString();
}

QString Generator::getType() {
    return meta->property("type").toString();
}

int Generator::getID() {
    return id;
}

QString Generator::getDescription() {
    return meta->property("description").toString();
}

QString Generator::getGeneratorName() {
    return generatorName;
}

QString Generator::getUserNotes() {
    return userNotes;
}

GeneratorMeta *Generator::getMeta() const
{
    return meta;
}

double Generator::getHistoryLatest() {
    return historyLatest;
}

bool Generator::getHistoryRefresher() {
    return historyRefresher;
}

int Generator::getOscInputPort() {
    return oscInputPort;
}

QString Generator::getOscInputAddress() {
    return oscInputAddress;
}

int Generator::getOscOutputPort() {
    return oscOutputPort;
}

QString Generator::getOscOutputAddressHost() {
    return oscOutputAddressHost;
}

QString Generator::getOscOutputAddressTarget() {
    return oscOutputAddressTarget;
}

int Generator::getInputCount() const
{
    return inputRegionSet->rowCount();
}

int Generator::getOutputCount() const
{
    return outputRegionSet->rowCount();
}

int Generator::getLatticeWidth() {
    return latticeWidth;
}

int Generator::getLatticeHeight() {
    return latticeHeight;
}

double Generator::getTimeScale() const
{
    return timeScale;
}

void Generator::writeGeneratorName(QString generatorName) {
    if (this->generatorName == generatorName)
        return;

    if (flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeGeneratorName (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << generatorName;
    }

    // check if new generatorName is valid -> must be unique
  QList<int> takenIDs;
    if(!AppModel::getInstance().validateNewGeneratorName(generatorName)) {

        // if name is not valid / unique, throw error / force to rename
        qDebug() << "throw error / require new gernatorName";
        return;
    }

    this->generatorName = generatorName;
    emit valueChanged("generatorName", QVariant(generatorName));
    emit generatorNameChanged(generatorName);
}

void Generator::writeUserNotes(QString userNotes) {
    if (this->userNotes == userNotes)
        return;

    if (flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeUserNotes (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << userNotes;
    }

    this->userNotes = userNotes;
    emit valueChanged("userNotes", QVariant(userNotes));
    emit userNotesChanged(userNotes);

}

void Generator::writeHistoryLatest(double historyLatest) {
    if(this->historyLatest == historyLatest) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeHistoryLatest (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << historyLatest;
    }

    this->historyLatest = historyLatest;
    emit valueChanged("historyLatest", QVariant(historyLatest));
    emit historyLatestChanged(historyLatest);
}

void Generator::flipHistoryRefresher() {
    historyRefresher = !historyRefresher;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "flipHistoryRefresher (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << historyRefresher;
    }

    emit valueChanged("historyRefresher", QVariant(historyRefresher));
    emit historyRefresherChanged(historyRefresher);
}

void Generator::writeOscInputPort(int oscInputPort) {
    if(this->oscInputPort == oscInputPort) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeOscInputPort (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << oscInputPort;
    }

    this->oscInputPort = oscInputPort;
    emit valueChanged("oscInputPort", QVariant(oscInputPort));
    emit oscReceiverPortChanged(oscInputPort);
}

void Generator::writeOscInputAddress(QString oscInputAddress) {
    if(this->oscInputAddress == oscInputAddress) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeOscInputAddress (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << oscInputAddress;
    }

    this->oscInputAddress = oscInputAddress;
    emit valueChanged("oscInputAddress", QVariant(oscInputAddress));
    emit oscInputAddressChanged(oscInputAddress);
}

void Generator::writeOscOutputPort(int oscOutputPort) {
    if(this->oscOutputPort == oscOutputPort) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeOscOutputPort (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << oscOutputPort;
    }

    this->oscOutputPort = oscOutputPort;
    emit valueChanged("oscOutputPort", QVariant(oscOutputPort));
    emit oscOutputPortChanged(oscOutputPort);
}

void Generator::writeOscOutputAddressHost(QString oscOutputAddressHost) {
    if(this->oscOutputAddressHost == oscOutputAddressHost) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeOscOutputAddressHost (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << oscOutputAddressHost;
    }

    this->oscOutputAddressHost = oscOutputAddressHost;
    emit valueChanged("oscOutputAddressHost", QVariant(oscOutputAddressHost));
    emit oscSenderHostChanged(oscOutputAddressHost);
}

void Generator::writeOscOutputAddressTarget(QString oscOutputAddressTarget) {
    if(this->oscOutputAddressTarget == oscOutputAddressTarget) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeOscOutputAddressTarget (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << oscOutputAddressTarget;
    }

    this->oscOutputAddressTarget = oscOutputAddressTarget;
    emit valueChanged("oscOutputAddressTarget", QVariant(oscOutputAddressTarget));
    emit oscOutputAddressTargetChanged(oscOutputAddressTarget);
}

void Generator::writeLatticeWidth(int latticeWidth) {
    if(this->latticeWidth == latticeWidth) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeLatticeWidth (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << latticeWidth;
    }

    // update property locally
    this->latticeWidth = latticeWidth;
    // re-initialize network
    initialize();

    emit valueChanged("latticeWidth", latticeWidth);
    emit latticeWidthChanged(latticeWidth);
}

void Generator::writeLatticeHeight(int latticeHeight) {
    if(this->latticeHeight == latticeHeight) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeLatticeHeight (Generator)\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << latticeHeight;
    }

    // update property locally
    this->latticeHeight = latticeHeight;
    // re-initialize network
    initialize();

    emit valueChanged("latticeHeight", latticeHeight);
    emit latticeHeightChanged(latticeHeight);
}

void Generator::writeTimeScale(double timeScale)
{
    if (this->timeScale == timeScale) {
        return;
    }

    // update property locally
    this->timeScale = timeScale;

    emit valueChanged("timeScale", timeScale);
    emit timeScaleChanged(timeScale);
}

void Generator::readJson(const QJsonObject &json)
{
    // 000. GENERAL DATA
    writeUserNotes(json["userNotes"].toString());


    // 001. PROP DATA
    const QMetaObject *metaObject = this->metaObject();
    QJsonObject props = json["props"].toObject();

    for (int i = 0; i < props.size(); i++) {
        // get key and value
        QString key = props.keys()[i];
        QVariant value = props[key].toVariant();

        // get index of property
        int index = metaObject->indexOfProperty(key.toStdString().c_str());

        // check if property exists; continue otherwise
        if (index == -1)
            continue;

        // write to property if it exists
        metaObject->property(index).write(this, value);
    }


    // 002. REGION DATA
    QJsonArray inputs = json["inputs"].toArray();
    QJsonArray outputs = json["outputs"].toArray();

    inputRegionSet->readJson(inputs);
    outputRegionSet->readJson(outputs);
}

void Generator::writeJson(QJsonObject &json) const
{
    // 000. GENERAL DATA
    json["id"] = id;
    json["type"] = meta->property("type").toString();
    json["userNotes"] = userNotes;


    // 001. PROP DATA
    QJsonObject props;

    // start at oscInputPort
    const QMetaObject *metaObject = this->metaObject();
    for (int i = metaObject->indexOfProperty("oscInputPort"); i < metaObject->propertyCount(); i++) {
        // retrieve target QMetaProperty
        QMetaProperty target = metaObject->property(i);

        // get key and value
        QString k = target.name();
        QVariant v = target.read(this);

        // typecast enum values
        // THIS CAN CAUSE BUGS IF THE ENUM ISN'T REGISTERED TO ENUMLABELS MEMBER!!!!!!
        if (meta->getEnumLabels().keys().contains(target.typeName()))
            v = v.toInt();

        // write to props obj
        props[k] = QJsonValue::fromVariant(v);
    }

    // write props to main JSON
    json["props"] = props;


    // 002. REGION DATA
    QJsonArray inputRegions;
    QJsonArray outputRegions;

    inputRegionSet->writeJson(inputRegions);
    outputRegionSet->writeJson(outputRegions);

    // write to main JSON
    json["inputs"] = inputRegions;
    json["outputs"] = outputRegions;
}

void Generator::resetParameters()
{
    const QMetaObject *metaObject = this->metaObject();

    for (int i = metaObject->indexOfProperty("latticeWidth"); i < metaObject->propertyCount(); i++) {
        QMetaProperty target = metaObject->property(i);
        // get property name
        QString underProp(metaObject->property(i).name());
        bool flag = false;

        // re-target prop if current target is a flag
        // (only flags can be exposed boolean properties, i've decided)
        if (QString(target.typeName()).compare("bool") == 0) {
            // remove "flag_" prefix
            underProp = underProp.mid(5, underProp.length());
            // check in
            flag = true;
        }

        // find associated default value for given property in the meta tree
        for (GeneratorField* f : meta->getFields()) {
            if (f->propName == underProp) {
                // check if default value is defined in the field data
                if (f->defaultValue.isNull()) break;

                // write default value to prop
                target.write(this, flag ? f->flagDefaultValue : f->defaultValue);

                // break to next field
                break;
            }
        }
    }

    // re-initialize
    // probably unnecessary, some properties can call this when rewritten
   initialize();
}

void Generator::resetRegions()
{
    // input rectangles reset
    inputRegionSet->deleteAllRegions();
    inputRegionSet->initialize();

    // output rectangles reset
    outputRegionSet->deleteAllRegions();
    outputRegionSet->initialize();
}

void Generator::initializeRegionSets()
{
    inputRegionSet->initialize();
    outputRegionSet->initialize();
}

void Generator::updateValue(const QString &key, const QVariant &value) {
    // qDebug() << "Generator::updateValue";

    QByteArray keyArray = key.toLocal8Bit();
    char* keyBuffer = keyArray.data();

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateValue (GeneratorFacade):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tkey: " << key << "\tvalue: " << value;
    }

    setProperty(keyBuffer, value);
}

void Generator::writeLatticeData(float** latticeData, int* allocatedWidth, int* allocatedHeight) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeLatticeData (Generator)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // try to lock the lattice data mutex
    bool lockSuccessful = latticeDataMutex.tryLock();

    if(!lockSuccessful) {
        if(flagDebug) {
            qDebug() << "writeLatticeData (Generator): failed to lock";
        }

        // failed to get lock, wait for GeneratorLatticeRenderer to finish its render method and let any other task get completed in the mean time
        QTimer timer;
        timer.setTimerType(Qt::PreciseTimer);
        timer.singleShot(0, this, [this, latticeData, allocatedWidth, allocatedHeight](){
            emit writeLatticeData(latticeData, allocatedWidth, allocatedHeight);
        });
    } else {
        if(flagDebug) {
            qDebug() << "writeLatticeData (Generator): succeded at getting lock";
        }

        // succeded at getting lock

        // check if the the right amount of memory is allocated
        if((*allocatedWidth) * (*allocatedHeight) != latticeWidth * latticeHeight) {
            // the amount of allocated memory mismatches the required amount, must reallocate

            // make sure this is not the first function call; in that case we can't safely delete since the pointer is uninitialized
            if(*latticeData != nullptr) {
                delete[] *latticeData;
            }

            // allocate the memory
            *latticeData = new float[latticeWidth * latticeHeight];
        }

        // update the allocated width and height variables in any case
        *allocatedWidth = latticeWidth;
        *allocatedHeight = latticeHeight;

        if(flagDebug) {
            qDebug() << "writeLatticeData (Generator): allocation done";
        }

        // write to the lattice data
        for(int x = 0; x < latticeWidth; x++) {
            for(int y = 0; y < latticeHeight; y++) {
                int index = x % latticeWidth + y * latticeWidth;
                (*latticeData)[index] = (float) getLatticeValue(x, y);
            }
        }

        if(flagDebug) {
            qDebug() << "writeLatticeData (Generator): write done";
        }

        // we are done; release the mutex
        latticeDataMutex.unlock();

        if(flagDebug) {
            qDebug() << "writeLatticeData (Generator): unlocked";
        }

        // we are done; tell the GeneratorLatticeCommunicator
        emit writeLatticeDataCompleted();
    }
}

void Generator::lockLatticeDataMutex() {
    latticeDataMutex.lock();
}

void Generator::unlockLatticeDataMutex() {
    latticeDataMutex.unlock();
}

void Generator::applyInputRegion() {
    // iterate over input regions
    for(int i = 0; i < inputRegionSet->rowCount(); i++) {
        GeneratorRegion* region = inputRegionSet->at(i);

        int xMax = region->getRect().x() + region->getRect().width();
        int yMax = region->getRect().y() + region->getRect().height();

        // write region activation onto lattice in rect area
        for(int x = region->getRect().x(); x < xMax; x++) {
            for(int y = region->getRect().y(); y < yMax; y++) {
                writeLatticeValue(x, y, region->getIntensity());
            }
        }
    }
}

void Generator::applyOutputRegion() {
    // iterate over output regions
    for(int i = 0; i < outputRegionSet->rowCount(); i++) {
        GeneratorRegion* region = outputRegionSet->at(i);

        int xMax = region->getRect().x() + region->getRect().width();
        int yMax = region->getRect().y() + region->getRect().height();

        double sum = 0;

        // collect lattice activations in rect area
        for(int x = region->getRect().x(); x < xMax; x++) {
            for(int y = region->getRect().y(); y < yMax; y++) {
                sum += getLatticeValue(x, y);
            }
        }

        // apply averaging
        sum /= (double) (region->getRect().width() * region->getRect().height());

        // write to region intensity
        region->writeIntensity(sum);
    }
}

GeneratorRegionSet* Generator::getInputRegionSet() const {
    return inputRegionSet.data();
}

GeneratorRegionSet* Generator::getOutputRegionSet() const {
    return outputRegionSet.data();
}
