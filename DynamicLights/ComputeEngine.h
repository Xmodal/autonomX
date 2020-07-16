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
#include <QTimer>
#include <QElapsedTimer>
#include <QList>
#include <QHash>
#include <QSharedPointer>
#include <random>
#include "Generator.h"


class ComputeEngine : public QObject {
    Q_OBJECT
private:
    QSharedPointer<QList<QSharedPointer<Generator>>> generators;
    QSharedPointer<QHash<int, QSharedPointer<Generator>>> generatorsHashMap;
    QElapsedTimer elapsedTimer;
    double frequency = 80;
    bool firstFrame = true;
    bool flagDebug = true;
    bool flagDummyOutputMonitor = false;
    bool flagDummyOscOutput = true;
    bool flagDisableProcessing = false;
    std::mt19937 randomGenerator;
std::uniform_real_distribution<> randomUniform;
public:
    ComputeEngine(QSharedPointer<QList<QSharedPointer<Generator>>> generators);
    ~ComputeEngine();
signals:
    void sendOscData(int id, QVariantList data);

    void createOscReceiver(int id, QString address, int port);
    void updateOscReceiver(int id, QString address, int port);
    void deleteOscReceiver(int id);

    void createOscSender(int id, QString addressHost, QString addressTarget, int port);
    void updateOscSender(int id, QString addressHost, QString addressTarget, int port);
    void deleteOscSender(int id);
public slots:
    void start();
    void loop();

    void recieveOscData(int id, QVariantList data);
};
