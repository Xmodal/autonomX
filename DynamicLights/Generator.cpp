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

Generator::Generator(QObject *parent) : QObject(parent) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (Generator)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
}


Generator::~Generator() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (Generator)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
}

void Generator::writeInput(double value, int index) {
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

QString Generator::getDescription() {
    return description;
}

double Generator::getOutputMonitor() {
    return outputMonitor;
}

void Generator::writeName(QString string) {
    if(name == string) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeName\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\t value = " << string;
    }

    name = string;
    emit valueChanged("name", QVariant(string));
    emit nameChanged(name);
}

void Generator::writeType(QString string) {
    if(type == string) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeType\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\t value = " << string;
    }

    type = string;
    emit valueChanged("type", QVariant(string));
    emit typeChanged(type);
}

void Generator::writeDescription(QString string) {
    if(description == string) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeDescription\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\t value = " << string;
    }

    description = string;
    emit valueChanged("description", QVariant(string));
    emit descriptionChanged(string);
}

void Generator::writeOutputMonitor(double value) {
    if(outputMonitor == value) {
        return;
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeOutputMonitor\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\t value = " << value;
    }

    outputMonitor = value;

    emit valueChanged("outputMonitor", QVariant(value));
    emit outputMonitorChanged(outputMonitor);
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
