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

#include "OscEngine.h"

#include <chrono>
#include <QDebug>
#include <QThread>

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

void OscEngine::connectReceiver(int id) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "connectReceiver (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id;
    }

    if(!oscReceivers.contains(id)) {
        throw std::runtime_error("osc receiver does not exist");
    }
    QSharedPointer<OscReceiver> receiver = oscReceivers.value(id);

    QObject::connect(receiver.data(), &OscReceiver::messageReceived, this, [this, id](const QString& oscAddress, const QVariantList& message){
        recieveOscDataHandler(id, oscAddress, message);
    });
}

void OscEngine::recieveOscDataHandler(int id, const QString& oscAddress, const QVariantList& message) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "recieveOscDataHandler (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\taddress = " << oscAddress << "\tmessage = " << message;
    }

    if(!oscReceivers.contains(id)) {
        throw std::runtime_error("osc receiver does not exist");
    }
    QString oscAddressExpected = oscReceiverAddresses.value(id);
    if(oscAddress == oscAddressExpected) {
        // message recieved with right address
        emit recieveOscData(id, message);
    }
}

void OscEngine::sendOscData(int id, QVariantList data) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "sendOscData (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\tmessage = " << data;
    }

    if(!oscSenders.contains(id)) {
        throw std::runtime_error("osc sender does not exist");
    }
    QSharedPointer<OscSender> sender = oscSenders.value(id);
    QString address = oscSenderAddresses.value(id);
    sender->send(address, data);
}

void OscEngine::createOscReceiver(int id, QString address, int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "createOscReceiver (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\taddress = " << address << "\tport = " << port;
    }

    if(oscReceivers.contains(id)) {
        throw std::runtime_error("osc receiver already exists");
    }
    QSharedPointer<OscReceiver> receiver = QSharedPointer<OscReceiver>(new OscReceiver(port));
    // update hash maps
    oscReceivers.insert(id, receiver);
    oscReceiverAddresses.insert(id, address);
    // connect reciever
    connectReceiver(id);
}

void OscEngine::deleteOscReceiver(int id) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "deleteOscReceiver (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id;
    }

    if(!oscReceivers.contains(id)) {
        throw std::runtime_error("osc receiver does not exist");
    }
    // delete from hash maps
    oscReceivers.remove(id);
    oscReceiverAddresses.remove(id);
}

void OscEngine::updateOscReceiverAddress(int id, QString address) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateOscReceiverAddress (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\taddress = " << address;
    }

    if(!oscReceivers.contains(id)) {
        throw std::runtime_error("osc receiver does not exist");
    }
    oscReceiverAddresses.insert(id, address);
}

void OscEngine::updateOscReceiverPort(int id, int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateOscReceiverPort (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\tport = " << port;
    }

    if(!oscReceivers.contains(id)) {
        throw std::runtime_error("osc receiver does not exist");
    }
    oscReceivers.value(id)->setPort(port);
}

void OscEngine::createOscSender(int id, QString addressHost, QString addressTarget, int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "createOscSender (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\taddressHost = " << addressHost << "\taddressTarget = " << addressTarget << "\tport = " << port;
    }

    if(oscSenders.contains(id)) {
        throw std::runtime_error("osc sender already exists");
    }
    QSharedPointer<OscSender> sender = QSharedPointer<OscSender>(new OscSender(addressHost, port));
    // update hash maps
    oscSenders.insert(id, sender);
    oscSenderAddresses.insert(id, addressTarget);
}

void OscEngine::deleteOscSender(int id) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "deleteOscSender (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id;
    }

    if(!oscSenders.contains(id)) {
        throw std::runtime_error("osc sender does not exist");
    }
    // delete from hash maps
    oscSenders.remove(id);
    oscSenders.remove(id);
}

void OscEngine::updateOscSenderAddressHost(int id, QString addressHost) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateOscSenderAddressHost (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\taddressHost = " << addressHost;
    }

    if(!oscSenders.contains(id)) {
        throw std::runtime_error("osc sender does not exist");
    }
    oscSenders.value(id)->setHostAddress(addressHost);
}

void OscEngine::updateOscSenderAddressTarget(int id, QString addressTarget) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateOscSenderAddressTarget (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\taddressTarget = " << addressTarget;
    }

    if(!oscSenders.contains(id)) {
        throw std::runtime_error("osc sender does not exist");
    }
    oscSenderAddresses.insert(id, addressTarget);
}

void OscEngine::updateOscSenderPort(int id, int port) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateOscSenderPort (OscEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\tport = " << port;
    }

    if(!oscSenders.contains(id)) {
        throw std::runtime_error("osc receiver does not exist");
    }
    oscSenders.value(id)->setPort(port);
}
