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
    QSharedPointer<QList<QSharedPointer<Generator>>> generatorsList;
    QSharedPointer<QHash<int, QSharedPointer<Generator>>> generatorsHashMap;
    QMap<QString, QString> parameterControlList;
    QElapsedTimer elapsedTimer;
    double frequency = 60;
    bool firstFrame = true;
    bool firstPass = true;
    bool inputValueReceived = false;
    bool flagDebug = false;
    bool flagDummyOutputMonitor = false;
    bool flagDummyOscOutput = false;
    bool flagDisableProcessing = false;
    bool flagCastOutputToFloat = true; // needed for Max as it doesn't support doubles
    std::mt19937 randomGenerator;
    std::uniform_real_distribution<> randomUniform;

public:
    ComputeEngine(QSharedPointer<QList<QSharedPointer<Generator>>> generatorsList, QSharedPointer<QHash<int, QSharedPointer<Generator>>> generatorsHashMap);
    ~ComputeEngine();

    QList<QString> registeredGeneratorTypes;
    void registerParameterControls(int generatorId);
//    void addGeneratorType(QString generatorType);
signals:
    // sends data through OscEngine::sendOscData
    void sendOscData(int generatorId, QVariantList data);
public slots:
    // handles data received from OscEngine::receiveOscData
    void receiveOscData(int generatorId, QVariantList data);
    void receiveOscGeneratorControlMessage(int generatorId, QVariantList data, QString controlMessage);


    // parses and directs received OSC messages that control generator parameters
//    void receiveGeneratorControlMessage(int generatorId, QVariantList data);

    // adds a generator to the list and hash map
    void addGenerator(QSharedPointer<Generator> generator);

    // removes a generator from the list and hash map
    void removeGenerator(QSharedPointer<Generator> generator);

    // starts the compute loop
    void start();

    // does the computation and schedules itself for the next iteration
    void loop();
};
