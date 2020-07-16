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

class OscEngine : public QObject {
    Q_OBJECT
public:
    OscEngine();
    ~OscEngine();
private:
    QHash<int, QSharedPointer<OscSender>> oscSenders;
    QHash<int, QString> oscSenderAddresses;
    QHash<int, QSharedPointer<OscReceiver>> oscReceivers;
    QHash<int, QString> oscReceiverAddresses;

    void connectReceiver(int id);

    bool flagDebug = true;
signals:
    void recieveOscData(int id, QVariantList data);
public slots:
    void recieveOscDataHandler(int id, const QString& oscAddress, const QVariantList& message);
    void sendOscData(int id, QVariantList data);

    void createOscReceiver(int id, QString address, int port);
    void updateOscReceiver(int id, QString address, int port);
    void deleteOscReceiver(int id);

    void createOscSender(int id, QString addressHost, QString addressTarget, int port);
    void updateOscSender(int id, QString addressHost, QString addressTarget, int port);
    void deleteOscSender(int id);
};
