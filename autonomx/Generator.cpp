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

#include "Generator.h"

Generator::Generator(int id, GeneratorMeta * meta) {
    this->id = id;
    this->meta = meta;
    this->userNotes = "";

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (Generator)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id;
    }

    inputRegionSet = QSharedPointer<GeneratorRegionSet>(new GeneratorRegionSet(0));
    outputRegionSet = QSharedPointer<GeneratorRegionSet>(new GeneratorRegionSet(1));
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

QString Generator::getUserNotes() {
    return userNotes;
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

int Generator::getLatticeWidth() {
    return latticeWidth;
}

int Generator::getLatticeHeight() {
    return latticeHeight;
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

    // qDebug() << userNotes << "\t" << this->userNotes;
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
    emit oscInputPortChanged(oscInputPort);
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
    emit oscOutputAddressHostChanged(oscOutputAddressHost);
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

void Generator::resetParameters()
{
    const QMetaObject *metaObject = this->metaObject();

    for (int i = metaObject->indexOfProperty("latticeWidth"); i < metaObject->propertyCount(); i++) {
        // get property object
        QMetaProperty metaProperty = metaObject->property(i);

        // TODO: flags need a different validation system
        // (hint: trim down w/o the "flag" prefix)

        // find associated default value for given property in the meta tree
        for (GeneratorField* f : meta->getFields()) {
            if (f->propName == metaProperty.name()) {
                // write default value to property
                // TODO: doesn't work with selects/enums yet
                metaProperty.write(this, f->defaultValue);

                // break to next field
                break;
            }
        }
    }

    // TODO: reset input/output zones as well...?

    // re-initialize
    initialize();
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
    for(int i = 0; i < inputRegionSet->getRegionCount(); i++) {
        GeneratorRegion* region = inputRegionSet->getRegion(i);

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
    for(int i = 0; i < inputRegionSet->getRegionCount(); i++) {
        GeneratorRegion* region = outputRegionSet->getRegion(i);

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
        region->writeMirroredIntensity(sum);
    }
}

GeneratorRegionSet* Generator::getInputRegionSet() {
    return inputRegionSet.data();
}

GeneratorRegionSet* Generator::getOutputRegionSet() {
    return outputRegionSet.data();
}
