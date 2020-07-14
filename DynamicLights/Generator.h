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

#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QSharedPointer>
#include <vector>

class Generator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE writeName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ getType NOTIFY typeChanged)
    Q_PROPERTY(QString description READ getDescription WRITE writeDescription NOTIFY descriptionChanged)
    Q_PROPERTY(double outputMonitor READ getOutputMonitor NOTIFY outputMonitorChanged)
protected:
    // the generator class provides input and output buffers
    std::vector<double> input;
    std::vector<double> output;

    // descriptive properties seen in the generators list panel
    QString name;           // generator name, assigned by user
    QString type;           // generator type, fixed
    QString description;    // generator description, fixed
    double outputMonitor;   // output monitor / indicator light, generated from output array automatically by ComputeEngine

    // example for indexing outputMonitorHistory:

    // indexing chronologically (oldest to newest)
    //
    //  for(int i = 0; i < outputMonitorHistorySizeValid; i++) {
    //      int index = (outputMonitorHistoryStartIndex + i) % outputMonitorHistorySizeMax;
    //  }
    //

    // indexing reverse-chronologically (newest to oldest)
    //
    //  for(int i = 0; i < outputMonitorHistorySizeValid; i++) {
    //      int index = (outputMonitorHistoryStartIndex + outputMonitorHistorySizeValid - 1 - i + outputMonitorHistorySizeMax) % outputMonitorHistorySizeMax;
    //  }
    //

public:
    explicit Generator(QObject *parent = nullptr);
    ~Generator();

    // the method implemented by the derived class that computes the output
    virtual void computeOutput(double deltaTime) = 0;

    // methods to read, write, and query the size of the io buffers
    void writeInput(double value, int index);
    double readOutput(int index);
    int getInputSize();
    int getOutputSize();

    // methods to read, write to descriptive properties seen in the generators list panel
    QString getName();
    QString getType();
    QString getDescription();
    double getOutputMonitor();

    void writeName(QString string);
    void writeType(QString string);
    void writeDescription(QString string);
    void writeOutputMonitor(double value);
private:
    bool flagDebug = false;
public slots:
    void updateValue(const QString &key, const QVariant &value); // for connection from QQmlPropertyMap's valueChanged signal
signals:
    void valueChanged(const QString &key, const QVariant &value); // for connection to QQmlPropertyMap's updateValue slot
    void nameChanged(QString);
    void typeChanged(QString);
    void descriptionChanged(QString);
    void outputMonitorChanged(double);
};
