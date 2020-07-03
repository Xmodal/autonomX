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

#include <iostream>
#include <chrono>
#include <QThread>

Generator::Generator(QObject *parent) : QObject(parent)
{

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

    name = string;
    emit valueChanged("name", QVariant(string));
    //emit nameChanged(name);
}

void Generator::writeType(QString string) {
    if(type == string) {
        return;
    }

    type = string;
    emit valueChanged("type", QVariant(string));
    //emit typeChanged(type);
}

void Generator::writeDescription(QString string) {
    if(description == string) {
        return;
    }

    description = string;
    emit valueChanged("description", QVariant(string));
    //emit descriptionChanged(string);
}

void Generator::writeOutputMonitor(double value) {
    if(outputMonitor == value) {
        return;
    }

    std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );


    std::cout << "writeOutputMonitor\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\t value = " << value << std::endl;

    outputMonitor = value;
    emit valueChanged("outputMonitor", QVariant(value));
    //emit outputMonitorChanged(outputMonitor);
}

void Generator::updateValue(const QString &key, const QVariant &value) {
    QByteArray array = key.toLocal8Bit();
    char* buffer = array.data();
    setProperty(buffer, value);
}
