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

    outputMonitorHistory = QSharedPointer<QVector<double>>(new QVector<double>(outputMonitorHistorySizeMax));
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

QSharedPointer<QVector<double>> Generator::getOutputMonitorHistory() {
    return outputMonitorHistory;
}

int Generator::getOutputMonitorHistoryStartIndex() {
    return outputMonitorHistoryStartIndex;
}

int Generator::getOutputMonitorHistorySizeMax() {
    return outputMonitorHistorySizeMax;
}

int Generator::getOutputMonitorHistorySizeValid() {
    return outputMonitorHistorySizeValid;
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

    // update history buffer
    if(outputMonitorHistorySizeValid == outputMonitorHistorySizeMax) {
        // buffer is full

        // index to write to is the previous start index
        int index = outputMonitorHistoryStartIndex;
        // write new value
        (*outputMonitorHistory)[index] = value;
        // increment start index
        outputMonitorHistoryStartIndex = (outputMonitorHistoryStartIndex + 1) % outputMonitorHistorySizeMax;
        // emit signals
        // emit outputMonitorHistoryChanged(outputMonitorHistory);
        emit valueChanged("outputMonitorHistoryStartIndex", outputMonitorHistoryStartIndex);
        emit outputMonitorHistoryStartIndexChanged(outputMonitorHistoryStartIndex);
    } else {
        // buffer is not full yet

        // index to write to is at start index offset by valid size
        int index = (outputMonitorHistoryStartIndex + outputMonitorHistorySizeValid) % outputMonitorHistorySizeMax;
        // write new value
        (*outputMonitorHistory)[index] = value;
        // increment valid size
        outputMonitorHistorySizeValid++;
        // emit signals
        // emit outputMonitorHistoryChanged(outputMonitorHistory);
        emit valueChanged("outputMonitorHistorySizeValid", outputMonitorHistorySizeValid);
        emit outputMonitorHistorySizeValidChanged(outputMonitorHistorySizeValid);
    }

    emit valueChanged("outputMonitor", QVariant(value));
    emit outputMonitorChanged(outputMonitor);

    /*
    qDebug() << "";
    for(int index = 0; index < outputMonitorHistorySizeMax; index++) {
        if((index >= outputMonitorHistoryStartIndex && index < outputMonitorHistoryStartIndex + outputMonitorHistorySizeValid) || (index + outputMonitorHistorySizeMax < outputMonitorHistoryStartIndex + outputMonitorHistorySizeValid)) {
            qDebug() << "index: " << index << "\tvalue: " << (*outputMonitorHistory)[index];
        } else {
            qDebug() << "index: " << index << "\tvalue: unassigned";
        }

    }
    */
}

void Generator::updateValue(const QString &key, const QVariant &value) {
    QByteArray array = key.toLocal8Bit();
    char* buffer = array.data();
    setProperty(buffer, value);
}
