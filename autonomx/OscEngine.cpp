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
#include <QDebug>
#include <QThread>

#include "OscEngine.h"

OscEngine::OscEngine() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
}

OscEngine::~OscEngine() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
}

void OscEngine::connectReceiver(int generatorId) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "connectReceiver (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId;
    }

//    if(!oscReceivers.contains(id)) {
//        throw std::runtime_error("osc receiver does not exist");
//    }
//    QSharedPointer<OscReceiver> receiver = oscReceivers.value(id);

    QObject::connect(receiver.data(), &OscReceiver::messageReceived, this, [this, generatorId](const QString& oscAddress, const QVariantList& message){
        receiveOscDataHandler(generatorId, oscAddress, message);
    });
}

void OscEngine::startGeneratorOsc(QSharedPointer<Generator> generator) {
    // get parameter values
    int generatorId = generator->getID();

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "startGeneratorOsc (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId;
    }

    QString addressReceiver = generator->getOscInputAddress();
    QString addressSenderHost = generator->getOscOutputAddressHost();
    QString addressSenderTarget = generator->getOscOutputAddressTarget();
//    int portReceiver = generator->getOscInputPort();
//    int portReceiver = 6668;
//    int portSender = 6669;

    createOscReceiver(generatorId, addressReceiver, oscReceiverPort);
    createOscSender(generatorId, addressSenderHost, addressSenderTarget, oscSenderPort);

    // connect input / receiver changes

//    QObject::connect(generator.data(), &Generator::oscInputAddressChanged, this, [this, id](QString oscInputAddress){
//        if(flagDebug) {
//            qDebug() << "oscInputAddressChanged (lambda)";
//        }
//        emit updateOscReceiverAddress(id, oscInputAddress);
//    });

    QObject::connect(generator.data(), &Generator::oscInputPortChanged, this, [this, generatorId](int oscInputPort){
        if(flagDebug) {
            qDebug() << "oscInputPortChanged (lambda)";
        }
        emit updateOscReceiverPort(generatorId, oscInputPort);
    });

    // connect output / sender changes
    QObject::connect(generator.data(), &Generator::oscOutputAddressHostChanged, this, [this, generatorId](QString oscOutputAddressHost){
        if(flagDebug) {
            qDebug() << "oscOutputAddressHostChanged (lambda)";
        }
        emit updateOscSenderHost(generatorId, oscOutputAddressHost);
    });

//    QObject::connect(generator.data(), &Generator::oscOutputAddressTargetChanged, this, [this, id](QString oscOutputAddressTarget){
//        if(flagDebug) {
//            qDebug() << "oscOutputAddressTargetChanged (lambda)";
//        }
//        emit updateOscSenderAddressTarget(id, oscOutputAddressTarget);
//    });

    QObject::connect(generator.data(), &Generator::oscOutputPortChanged, this, [this, generatorId](int oscOutputPort){
        if(flagDebug) {
            qDebug() << "oscOutputAddressTargetChanged (lambda)";
        }
        emit updateOscSenderPort(generatorId, oscOutputPort);
    });
}

void OscEngine::stopGeneratorOsc(QSharedPointer<Generator> generator) {
    int generatorId = generator->getID();

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "stopGeneratorOsc (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId;
    }

//    deleteOscReceiver(id);
    deleteOscSender(generatorId);
}

void OscEngine::receiveOscDataHandler(int generatorId, const QString& oscAddress, const QVariantList& message) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "receiveOscDataHandler (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\taddress = " << oscAddress << "\tmessage = " << message;
    }

//    if(!oscReceivers.contains(id)) {
//        throw std::runtime_error("osc receiver does not exist");
//    }
    QString oscAddressExpected = oscReceiverAddress;
    if(oscAddress == oscAddressExpected) {
        // message received with right address
        emit receiveOscData(generatorId, message);
    }
}

void OscEngine::updateValue(const QString &key, const QVariant &value)
{
    QByteArray keyArray = key.toLocal8Bit();
    char* keyBuffer = keyArray.data();

    setProperty(keyBuffer, value);
}

void OscEngine::sendOscData(int generatorId, QVariantList data) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "sendOscData (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\tmessage = " << data;
    }

    if(!oscSenders.contains(generatorId)) {
        // we allow this to happen without an exception because this can occur when deleting a generator.
        // there is a race condition between the deletion of the Generator and its associated OscSender when AppModel orders their respective threads (computeThread and oscThread) to delete them later.
        // if the OscSender is deleted first and the ComputeEngine renders an iteration before it deletes the Generator, it will attempt to send a message through an OscSender that doesn't exist.
        return;
    }
    QSharedPointer<OscSender> sender = oscSenders.value(generatorId);
//    QString address = oscSenderAddresses.value(id);
//    qDebug() << "osc sender address = " << address;
    sender->send(oscReceiverAddress, data);
}

void OscEngine::createOscReceiver(int generatorId, QString address, int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "createOscReceiver (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\taddress = " << address << "\tport = " << port;
    }

//    if(oscReceivers.contains(id)) {
//        throw std::runtime_error("osc receiver already exists");
//    }

    if(createOscReceiverBoolean) {
        receiver = QSharedPointer<OscReceiver>(new OscReceiver(port));

        createOscReceiverBoolean = false;
    }

    // update hash maps
//    oscReceivers.insert(id, receiver);
//    oscReceiverAddresses.insert(id, address);
    // connect receiver
    connectReceiver(generatorId);
}

//void OscEngine::deleteOscReceiver(int id) {
//    if(flagDebug) {
//        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
//                    std::chrono::system_clock::now().time_since_epoch()
//        );

//        qDebug() << "deleteOscReceiver (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id;
//    }

////    if(!oscReceivers.contains(id)) {
////        throw std::runtime_error("osc receiver does not exist");
////    }
//    // delete from hash maps
////    oscReceivers.remove(id);
//    oscReceiverAddresses.remove(id);
//}

//void OscEngine::updateOscReceiverAddress(int id, QString address) {
//    if(flagDebug) {
//        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
//                    std::chrono::system_clock::now().time_since_epoch()
//        );

//        qDebug() << "updateOscReceiverAddress (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\taddress = " << address;
//    }

////    if(!oscReceivers.contains(id)) {
////        // we allow this to happen without an exception because this can occur when deleting a generator.
////        // there is a race condition between the deletion of the Generator and its associated OscReceiver when AppModel orders their respective threads (computeThread and oscThread) to delete them later.
////        // this makes it possible to attempt to edit the properties of an OscReceiver that doesn't exist.
////        return;
////    }
//    oscReceiverAddresses.insert(id, address);
//}

void OscEngine::updateOscReceiverPort(int generatorId, int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateOscReceiverPort (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\tport = " << port;
    }

//    if(!oscReceivers.contains(id)) {
//        // we allow this to happen without an exception because this can occur when deleting a generator.
//        // there is a race condition between the deletion of the Generator and its associated OscReceiver when AppModel orders their respective threads (computeThread and oscThread) to delete them later.
//        // this makes it possible to attempt to edit the properties of an OscReceiver that doesn't exist.
//        return;
//    }
//    oscReceivers.value(id)->setPort(port);
    oscReceiverPort = port;

//    receiver->setPort(port);
}

void OscEngine::createOscSender(int generatorId, QString addressHost, QString addressTarget, int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "createOscSender (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\taddressHost = " << addressHost << "\taddressTarget = " << addressTarget << "\tport = " << port;
    }

    if(oscSenders.contains(generatorId)) {
        throw std::runtime_error("osc sender already exists");
    }
    QSharedPointer<OscSender> sender = QSharedPointer<OscSender>(new OscSender(addressHost, port));
    // update hash maps
    oscSenders.insert(generatorId, sender);
//    oscSenderAddresses.insert(id, addressTarget);
}

void OscEngine::deleteOscSender(int generatorId) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "deleteOscSender (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId;
    }

    if(!oscSenders.contains(generatorId)) {
        throw std::runtime_error("osc sender does not exist");
    }
    // delete from hash maps
    oscSenders.remove(generatorId);
    oscSenders.remove(generatorId);
}

int OscEngine::getOscReceiverPort() const
{
    return this->oscReceiverPort;
}

int OscEngine::getOscSenderPort() const
{
    return this->oscSenderPort;
}

QString OscEngine::getOscSenderHost() const
{
    return this->OscSenderHost;
}

void OscEngine::writeOscReceiverPort(int port)
{
    this->oscReceiverPort = port;
    emit OscReceiverPortChanged(port);
}

void OscEngine::writeOscSenderPort(int port)
{
    this->oscSenderPort = port;
    emit OscSenderPortChanged(port);
}

void OscEngine::writeOscSenderHost(QString host)
{
    this->OscSenderHost = host;
    emit OscSenderHostChanged(host);
}

void OscEngine::updateOscSenderHost(int generatorId, QString host) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateOscSenderAddressHost (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\taddressHost = " << host;
    }

    if(!oscSenders.contains(generatorId)) {
        // we allow this to happen without an exception because this can occur when deleting a generator.
        // there is a race condition between the deletion of the Generator and its associated OscSender when AppModel orders their respective threads (computeThread and oscThread) to delete them later.
        // this makes it possible to attempt to edit the properties of an OscSender that doesn't exist.
        return;
    }
    oscSenders.value(generatorId)->setOscSenderHost(host);
}

//void OscEngine::updateOscSenderAddressTarget(int id, QString addressTarget) {
//    if(flagDebug) {
//        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
//                    std::chrono::system_clock::now().time_since_epoch()
//        );

//        qDebug() << "updateOscSenderAddressTarget (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\taddressTarget = " << addressTarget;
//    }

////    if(!oscSenders.contains(id)) {
////        // we allow this to happen without an exception because this can occur when deleting a generator.
////        // there is a race condition between the deletion of the Generator and its associated OscSender when AppModel orders their respective threads (computeThread and oscThread) to delete them later.
////        // this makes it possible to attempt to edit the properties of an OscSender that doesn't exist.
////        return;
////    }
////    oscSenderAddresses.insert(id, addressTarget);

//    oscSenderAddress = addressTarget;
//}

void OscEngine::updateOscSenderPort(int generatorId, int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateOscSenderPort (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\tport = " << port;
    }

//    if(!oscSenders.contains(id)) {
//        // we allow this to happen without an exception because this can occur when deleting a generator.
//        // there is a race condition between the deletion of the Generator and its associated OscSender when AppModel orders their respective threads (computeThread and oscThread) to delete them later.
//        // this makes it possible to attempt to edit the properties of an OscSender that doesn't exist.
//        return;
//    }
//    oscSenders.value(id)->setPort(port);

    QHash<int, QSharedPointer<OscSender>>::iterator i;
    for(i = oscSenders.begin(); i !=oscSenders.end(); ++i) {
        i.value()->setPort(oscSenderPort);
    }
}
