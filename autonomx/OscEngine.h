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
    Q_PROPERTY(int oscReceiverPort READ getOscReceiverPort WRITE writeOscReceiverPort NOTIFY OscReceiverPortChanged)
    Q_PROPERTY(int oscSenderPort READ getOscSenderPort WRITE writeOscSenderPort NOTIFY OscSenderPortChanged)
    Q_PROPERTY(QString oscSenderHost READ getOscSenderHost WRITE writeOscSenderHost NOTIFY OscSenderHostChanged)
public:
    OscEngine();
    ~OscEngine();
private:
    QHash<int, QSharedPointer<OscSender>> oscSenders;
    QSharedPointer<OscReceiver> oscReceiver;
    int oscSenderPort = 6669;
    int oscReceiverPort = 6668;
    QString oscSenderHost = "127.0.0.1";
    bool createOscReceiverBoolean = true;

    // connects OscReceiver::messageReceived to OscEngine::receiveOscDataHandler through a lambda that captures the generator id
    void connectReceiver(int generatorId);

    // used internally by connectGenerator and disconnectGenerator
    void createOscReceiver(int generatorId, QString address, int port);

    // used internally by connectGenerator and disconnectGenerator
    void createOscSender(int generatorId, QString addressHost, QString addressTarget, int port);
    void deleteOscSender(int generatorId);

    // getters
    int getOscReceiverPort() const;
    int getOscSenderPort() const;
    QString getOscSenderHost() const;

    // setters
    void writeOscReceiverPort(int port);
    void writeOscSenderPort(int port);
    void writeOscSenderHost(QString host);

    bool flagDebug = false;

signals:
    // to facade
    void valueChanged(const QString &key, const QVariant &value);

    // relays updates to ComputeEngine::rec
    void receiveOscData(int generatorId, QVariantList data, QString generatorAddress);
    // relays generator control messages to ComputeEngine::rec
    void receiveOscGeneratorControlMessage(int generatorId, QVariantList data, QString controlMessage);

    // notifiers
    void OscReceiverPortChanged(int receiverPort);
    void OscSenderPortChanged(int senderPort);
    void OscSenderHostChanged(QString host);

private slots:
    // bridges OscReceiver::messageReceived to OscEngine::receiveOscData
    void receiveOscDataHandler(int generatorId, const QString& oscAddress, const QVariantList& message, bool controlMessageBool);

public slots:
    // from facade
    void updateValue(const QString &key, const QVariant &value);

    // starts processing for a generator using createOscReceiver and createOscSender and setups lambda connections from the generators to the updateOsc... methods. emitted from AppModel
    void startGeneratorOsc(QSharedPointer<Generator> generator);
    // stops processing for a generator using deleteOscReceiver and deleteOscSender. emitted by AppModel
    void stopGeneratorOsc(QSharedPointer<Generator> generator);

    // bridges ComputeEngine::sendOscData to OscSender::send
    void sendOscData(int generatorId, const QString& generatorName, const QString& attributeName, const QVariantList& arguments);

    // updates receiver parameters
    void updateOscReceiverPort(int port);

    // updates receiver parameters
    void updateOscSenderHost(QString host);
    void updateOscSenderPort(int port);
};
