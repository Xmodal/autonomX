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
#include <QDebug>

#include <chrono>
#include <QThread>
#include <QDebug>

Generator::Generator(int id) {
    this->id = id;

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

int Generator::getId() {
    return id;
}

QString Generator::getName() {
    return name;
}

QString Generator::getType() {
    return type;
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
    emit oscInputUpdated(oscInputAddress, oscInputPort);
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
    emit oscInputUpdated(oscInputAddress, oscInputPort);
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
    emit oscOutputUpdated(oscOutputAddressHost, oscOutputAddressTarget, oscInputPort);
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
    emit oscOutputUpdated(oscOutputAddressHost, oscOutputAddressTarget, oscInputPort);
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
    emit oscOutputUpdated(oscOutputAddressHost, oscOutputAddressTarget, oscInputPort);
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
