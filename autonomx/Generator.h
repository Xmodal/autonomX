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
#include <QColor>
#include <vector>

class Generator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE writeName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ getType NOTIFY typeChanged)
    Q_PROPERTY(int id READ getID)
    Q_PROPERTY(QString description READ getDescription WRITE writeDescription NOTIFY descriptionChanged)
    Q_PROPERTY(double outputMonitor READ getOutputMonitor NOTIFY outputMonitorChanged)

    Q_PROPERTY(int oscInputPort READ getOscInputPort WRITE writeOscInputPort NOTIFY oscInputPortChanged)
    Q_PROPERTY(QString oscInputAddress READ getOscInputAddress WRITE writeOscInputAddress NOTIFY oscInputAddressChanged)

    Q_PROPERTY(int oscOutputPort READ getOscOutputPort WRITE writeOscOutputPort NOTIFY oscOutputPortChanged)
    Q_PROPERTY(QString oscOutputAddressHost READ getOscOutputAddressHost WRITE writeOscOutputAddressHost NOTIFY oscOutputAddressHostChanged)
    Q_PROPERTY(QString oscOutputAddressTarget READ getOscOutputAddressTarget WRITE writeOscOutputAddressTarget NOTIFY oscOutputAddressTargetChanged)
protected:
    int id;                         // generator id, generated automatically by ComputeEngine in constructor

    // descriptive properties seen in the generators list panel
    QString name;                   // generator name, assigned by user
    QString type;                   // generator type, fixed
    QString description;            // generator description, fixed
    double outputMonitor;           // output monitor / indicator light, generated from output array automatically by ComputeEngine

    int oscInputPort = 6668;                    // generator osc input port, assigned by user
    QString oscInputAddress = "/input";         // generator osc input address, assigned by user (this is an osc destination)

    int oscOutputPort = 6669;                   // generator osc output port, assigned by user
    QString oscOutputAddressHost = "127.0.0.1"; // generator osc output address for host, assigned by user (this is an ip)
    QString oscOutputAddressTarget = "/output"; // generator osc output address for target, assigned by user (this is an osc destination)
public:
    Generator(int id);
    ~Generator();

    // the generator class provides input and output buffers
    // these are resized automatically by the class deriving generator
    std::vector<double> input;
    std::vector<double> output;

    // methods to read, write, and query the size of the io buffers
    void writeInput(double value, int index);
    double readOutput(int index);
    int getInputSize();
    int getOutputSize();

    // the method implemented by the derived class that computes the output
    virtual void computeOutput(double deltaTime) = 0;

    // methods to read properties
    QString getName();
    QString getType();
    int getID();
    QString getDescription();
    double getOutputMonitor();

    int getOscInputPort();
    QString getOscInputAddress();

    int getOscOutputPort();
    QString getOscOutputAddressHost();
    QString getOscOutputAddressTarget();

    // methods to write properties
    void writeName(QString name);
    void writeType(QString type);
    void writeDescription(QString description);
    void writeOutputMonitor(double outputMonitor);

    void writeOscInputPort(int oscInputPort);
    void writeOscInputAddress(QString oscInputAddress);

    void writeOscOutputPort(int oscOutputPort);
    void writeOscOutputAddressHost(QString oscOutputAddressHost);
    void writeOscOutputAddressTarget(QString oscOutputAddressTarget);
private:
    bool flagDebug = false;
public slots:
    // common slot allowing to update any property. allows the Facade class to work properly
    // (for connection from QQmlPropertyMap's valueChanged signal)
    void updateValue(const QString &key, const QVariant &value);
signals:
    // common signal used alongside all other property change signals. allows the Facade class to work properly
    // (for connection to QQmlPropertyMap's updateValue slot)
    void valueChanged(const QString &key, const QVariant &value);

    // usual signals for property changes
    void nameChanged(QString name);
    void typeChanged(QString type);
    void descriptionChanged(QString description);
    void outputMonitorChanged(double outputMonitor);

    void oscInputPortChanged(int oscInputPort);
    void oscInputAddressChanged(QString oscInputAddress);

    void oscOutputPortChanged(int oscOutputPort);
    void oscOutputAddressHostChanged(QString oscOutputAddressHost);
    void oscOutputAddressTargetChanged(QString oscOutputAddressTarget);
};
