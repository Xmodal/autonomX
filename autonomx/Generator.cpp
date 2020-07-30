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

#include "Generator.h"

#include <chrono>
#include <QThread>
#include <QDebug>

Generator::Generator(int id, QString name, QString type, QString description) {
    this->id = id;
    this->name = name;
    this->type = type;
    this->description = description;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (Generator)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id;
    }
}


Generator::~Generator() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (Generator)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id;
    }
}

void Generator::writeInput(double value, int index) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeInput (Generator)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\tinid = " << index << "\tvalue = " << value;
    }

    input[index] = value;
}

double Generator::readOutput(int index) {
    return output[index];
}

int Generator::getInputSize() {
    return input.size();
}

int Generator::getOutputSize() {
    return output.size();
}

QString Generator::getName() {
    return name;
}

QString Generator::getType() {
    return type;
}

int Generator::getID() {
    return id;
}

QString Generator::getDescription() {
    return description;
}

double Generator::getOutputMonitor() {
    return outputMonitor;
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

void Generator::writeName(QString name) {
    if(this->name == name) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeName\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << name;
    }

    this->name = name;
    emit valueChanged("name", QVariant(name));
    emit nameChanged(name);
}

void Generator::writeType(QString type) {
    if(this->type == type) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeType\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << type;
    }

    this->type = type;
    emit valueChanged("type", QVariant(type));
    emit typeChanged(type);
}

void Generator::writeDescription(QString description) {
    if(this->description == description) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeDescription\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << description;
    }

    this->description = description;
    emit valueChanged("description", QVariant(description));
    emit descriptionChanged(description);
}

void Generator::writeOutputMonitor(double outputMonitor) {
    if(this->outputMonitor == outputMonitor) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeOutputMonitor\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << outputMonitor;
    }

    this->outputMonitor = outputMonitor;
    emit valueChanged("outputMonitor", QVariant(outputMonitor));
    emit outputMonitorChanged(outputMonitor);
}

void Generator::writeOscInputPort(int oscInputPort) {
    if(this->oscInputPort == oscInputPort) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeOscInputPort\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << oscInputPort;
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

        qDebug() << "writeOscInputAddress\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << oscInputAddress;
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

        qDebug() << "writeOscOutputPort\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << oscOutputPort;
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

        qDebug() << "writeOscOutputAddressHost\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << oscOutputAddressHost;
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

        qDebug() << "writeOscOutputAddressTarget\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << oscOutputAddressTarget;
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

        qDebug() << "writeLatticeWidth\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << latticeWidth;
    }

    // actually do the changes in the derived algorithm
    writeLatticeWidthDelegate(latticeWidth);

    this->latticeWidth = latticeWidth;
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

        qDebug() << "writeLatticeHeight\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\t value = " << latticeHeight;
    }

    // actually do the changes in the derived algorithm
    writeLatticeHeightDelegate(latticeHeight);

    this->latticeHeight = latticeHeight;
    emit valueChanged("latticeHeight", latticeHeight);
    emit latticeHeightChanged(latticeHeight);
}

void Generator::updateValue(const QString &key, const QVariant &value) {
    QByteArray keyArray = key.toLocal8Bit();
    char* keyBuffer = keyArray.data();

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateValue (" << keyBuffer << ") (in Generator)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\t value = " << value;
    }

    setProperty(keyBuffer, value);
}

void Generator::writeLatticeData(double* latticeData, int* allocatedWidth, int* allocatedHeight) {
    // try to lock the lattice data mutex
    bool lockSuccessful = latticeDataMutex.tryLock();

    if(!lockSuccessful) {
        // failed to get lock, wait for GeneratorLatticeRenderer to finish its render method and let any other task get completed in the mean time
        emit writeLatticeData(latticeData, allocatedWidth, allocatedHeight);
    } else {
        // succeded at getting lock

        // check if the the right amount of memory is allocated
        if((*allocatedWidth) * (*allocatedHeight) == latticeWidth * latticeHeight) {
            // the same amount of data is allocated, but the aspect ratio could be different
        } else {
            // the amount of allocated memory mismatches the required amount, must reallocate
            delete[] latticeData;
            latticeData = new double[latticeWidth * latticeHeight];
        }

        // update the allocated width and height variables in any case
        *allocatedWidth = latticeWidth;
        *allocatedHeight = latticeHeight;

        // we can now write to the lattice data. do it via the derived delegate
        writeLatticeDataDelegate(latticeData);

        // we are done; release the mutex
        latticeDataMutex.unlock();

        // we are done; tell the GeneratorLatticeRenderer
        emit writeLatticeDataCompleted();
    }
}
