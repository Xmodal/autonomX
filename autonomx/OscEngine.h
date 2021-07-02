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
#include <QSharedPointer>
#include <QHash>

#include "OscSender.h"
#include "OscReceiver.h"
#include "Generator.h"

class OscEngine : public QObject {
    Q_OBJECT
    Q_PROPERTY(int receiverPort READ getReceiverPort WRITE writeReceiverPort NOTIFY receiverPortChanged)
    Q_PROPERTY(int senderPort READ getSenderPort WRITE writeSenderPort NOTIFY senderPortChanged)
    Q_PROPERTY(QString senderHostAddress READ getSenderHostAddress WRITE writeSenderHostAddress NOTIFY senderHostAddressChanged)
public:
    OscEngine();
    ~OscEngine();
private:
    QHash<int, QSharedPointer<OscSender>> oscSenders;
//    QHash<int, QString> oscSenderAddresses;
    QString oscSenderAddress = "/output";
//    QHash<int, QSharedPointer<OscReceiver>> oscReceivers;
    QHash<int, QString> oscReceiverAddresses;
    QString oscReceiverAddress = "/input";
    bool createOscReceiverBoolean = true;
    QSharedPointer<OscReceiver> receiver;
    int oscReceiverPort = 6668;
    int oscSenderPort = 6669;
    QString senderHostAddress = "127.0.0.1";

    // connects OscReceiver::messageReceived to OscEngine::receiveOscDataHandler through a lambda that captures the generator id
    void connectReceiver(int id);

    // used internally by connectGenerator and disconnectGenerator
    void createOscReceiver(int id, QString address, int port);
    void deleteOscReceiver(int id);

    // used internally by connectGenerator and disconnectGenerator
    void createOscSender(int id, QString addressHost, QString addressTarget, int port);
    void deleteOscSender(int id);

    bool flagDebug = false;

    // getters
    int getReceiverPort() const;
    int getSenderPort() const;
    QString getSenderHostAddress() const;

    // setters
    void writeReceiverPort(int port);
    void writeSenderPort(int port);
    void writeSenderHostAddress(QString address);
signals:
    // to facade
    void valueChanged(const QString &key, const QVariant &value);

    // relays updates to ComputeEngine::rec
    void receiveOscData(int id, QVariantList data);

    // notifiers
    void receiverPortChanged(int receiverPort);
    void senderPortChanged(int senderPort);
    void senderHostAddressChanged(QString senderHostAddress);
private slots:
    // bridges OscReceiver::messageReceived to OscEngine::receiveOscData
    void receiveOscDataHandler(int id, const QString& oscAddress, const QVariantList& message);
public slots:
    // from facade
    void updateValue(const QString &key, const QVariant &value);

    // starts processing for a generator using createOscReceiver and createOscSender and setups lambda connections from the generators to the updateOsc... methods. emitted from AppModel
    void startGeneratorOsc(QSharedPointer<Generator> generator);
    // stops processing for a generator using deleteOscReceiver and deleteOscSender. emitted by AppModel
    void stopGeneratorOsc(QSharedPointer<Generator> generator);

    // bridges ComputeEngine::sendOscData to OscSender::send
    void sendOscData(int id, QVariantList data);

    // updates receiver parameters
    void updateOscReceiverAddress(int id, QString address);
    void updateOscReceiverPort(int id, int port);

    // updates receiver parameters
    void updateOscSenderAddressHost(int id, QString addressHost);
    void updateOscSenderAddressTarget(int id, QString addressHost);
    void updateOscSenderPort(int id, int port);
};
