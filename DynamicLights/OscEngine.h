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
private:
    QHash<int, QSharedPointer<OscSender>> oscSenders;
    QHash<int, QSharedPointer<OscReceiver>> oscRecievers;
signals:
    void dataRecieved(int, QVariant);
public slots:
    void sendData(int id, QVariant data);

    void createReciever(int id, QString address, int port);
    void updateReciever(int id, QString address, int port);
    void deleteReciever(int id);

    void createSender(int id, QString hostAddress, int port);
    void updateSender(int id, QString hostAddress, int port);
    void deleteSender(int id);
};
