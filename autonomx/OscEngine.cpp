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

    QObject::connect(oscReceiver.data(), &OscReceiver::messageReceived, this, [this, generatorId](const QString& oscAddress, const QVariantList& message, bool controlMessageBool){
        receiveOscDataHandler(generatorId, oscAddress, message, controlMessageBool);
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

    createOscReceiver(generatorId, addressReceiver, oscReceiverPort);
    createOscSender(generatorId, addressSenderHost, addressSenderTarget, oscSenderPort);

    // connect oscReceiver object port to generator(s)
    QObject::connect(generator.data(), &Generator::oscReceiverPortChanged, this, [this](int oscReceiverPort){
        if(flagDebug) {
            qDebug() << "oscInputPortChanged (lambda)";
        }
        emit updateOscReceiverPort(oscReceiverPort);
    });

    // connect oscSender object host to generator(s)
    QObject::connect(generator.data(), &Generator::oscSenderHostChanged, this, [this](QString oscSenderHost){
        if(flagDebug) {
            qDebug() << "oscOutputAddressHostChanged (lambda)";
        }
        emit updateOscSenderHost(oscSenderHost);
    });

    // connect oscSender object port to generator(s)
    QObject::connect(generator.data(), &Generator::oscOutputPortChanged, this, [this](int oscSenderPort){
        if(flagDebug) {
            qDebug() << "oscOutputAddressTargetChanged (lambda)";
        }
        emit updateOscSenderPort(oscSenderPort);
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

    deleteOscSender(generatorId);
}

void OscEngine::receiveOscDataHandler(int generatorId, const QString& oscAddress, const QVariantList& message, bool controlMessageBool) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "receiveOscDataHandler (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\taddress = " << oscAddress << "\tmessage = " << message;
    }

    if(!controlMessageBool) {
            emit receiveOscData(generatorId, message);
    } else if(controlMessageBool) {
            QString generatorControlMessage = oscAddress;
            emit receiveOscGeneratorControlMessage(generatorId, message, generatorControlMessage);
    }
}

void OscEngine::updateValue(const QString &key, const QVariant &value) {
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
    sender->send(oscReceiverAddress, data);
}

void OscEngine::createOscReceiver(int generatorId, QString address, int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );
        qDebug() << "createOscReceiver (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << generatorId << "\taddress = " << address << "\tport = " << port;
    }

    // check if it's the first time creating a receiver object
    // only one osc receiver object needed
    if(createOscReceiverBoolean) {
        oscReceiver = QSharedPointer<OscReceiver>(new OscReceiver(port));
        createOscReceiverBoolean = false;
    }

    // connect receiver
    connectReceiver(generatorId);
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
}

int OscEngine::getOscReceiverPort() const {
    return this->oscReceiverPort;
}

int OscEngine::getOscSenderPort() const {
    return this->oscSenderPort;
}

QString OscEngine::getOscSenderHost() const {
    return this->oscSenderHost;
}

void OscEngine::writeOscReceiverPort(int port) {
    // change global receiverPort variable
    this->oscReceiverPort = port;
    // update oscReceiver object to global value
    updateOscReceiverPort(port);
    emit OscReceiverPortChanged(port);
}

void OscEngine::updateOscReceiverPort(int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );
        qDebug() << "updateOscReceiverPort (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tport = " << port;
    }

    oscReceiver->setPort(port);
}

void OscEngine::writeOscSenderPort(int port) {
    // change global senderPort variable
    this->oscSenderPort = port;
    // update all oscSender objects to global value
    updateOscSenderPort(port);
    emit OscSenderPortChanged(port);
}

void OscEngine::updateOscSenderPort(int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );
        qDebug() << "updateOscSenderPort (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tport = " << port;
    }

    QHash<int, QSharedPointer<OscSender>>::iterator i;
    for(i = oscSenders.begin(); i !=oscSenders.end(); ++i) {
        i.value()->setPort(oscSenderPort);
    }
}

void OscEngine::writeOscSenderHost(QString host) {
    // change global host variable
    this->oscSenderHost = host;
    // update all osc sender objects to global value
    updateOscSenderHost(host);
    emit OscSenderHostChanged(host);
}

void OscEngine::updateOscSenderHost(QString host) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );
        qDebug() << "updateOscSenderAddressHost (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\taddressHost = " << host;
    }

    QHash<int, QSharedPointer<OscSender>>::iterator i;
    for(i = oscSenders.begin(); i !=oscSenders.end(); ++i) {
        i.value()->setOscSenderHost(host);
    }
}
