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

#include "ComputeEngine.h"

#include <algorithm>
#include <chrono>
#include <QDebug>
#include <QThread>


ComputeEngine::ComputeEngine(QSharedPointer<QList<QSharedPointer<Generator>>> generators) : randomUniform(0.0, 1.0) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (ComputeEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->generators = generators;
    generatorsHashMap = QSharedPointer<QHash<int, QSharedPointer<Generator>>>(new QHash<int, QSharedPointer<Generator>>);

    for(QList<QSharedPointer<Generator>>::iterator it = generators->begin(); it != generators->end(); it++) {
        // get generator info
        QSharedPointer<Generator> generator = *it;
        // create hash map entry
        generatorsHashMap.get()->insert(generator->getId(), generator);
    }
}

ComputeEngine::~ComputeEngine() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );// get generator

        qDebug() << "destructor (ComputeEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    for(QList<QSharedPointer<Generator>>::iterator it = generators->begin(); it != generators->end(); it++) {
        // get generator info
        QSharedPointer<Generator> generator = *it;
        int id = generator->getId();
        // delete osc sender and receiver
        emit deleteOscReceiver(id);
        emit deleteOscSender(id);
    }
}

void ComputeEngine::recieveOscData(int id, QVariantList data) {
    if(!generatorsHashMap->contains(id)) {
        throw std::runtime_error("generator does not exist");
    }
    QSharedPointer<Generator> generator = generatorsHashMap->value(id);

    QList<QVariant> dataAsList = data;

    int argumentsTotal = data.size();
    int argumentsValid = 0;

    for(int i = 0; i < generator->getInputSize(); i++) {
        // set default to 0
        double input = 0;
        // check that the message's list is long enough
        if(i < data.size()) {
            // check the message's type can be cast to double
            QMetaType::Type type = (QMetaType::Type) dataAsList.at(i).type();
            if(type == QMetaType::Float || QMetaType::Double || QMetaType::Int || QMetaType::Long) {
                input = dataAsList.at(i).toDouble();
                argumentsValid++;
            }
        }
        // write to input
        generator->writeInput(input, i);
    }

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "recieveOscData (ComputeEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tgenid = " << id << "\tdata = " << data << "\t(" << argumentsValid << " of " << argumentsTotal << " valid)";
    }
}

void ComputeEngine::start() {
    // this is called from an external thread initially and allows this object's thread to pick up the loop statement through its event queue
    // having a singleshot timer with time zero executes as soon as possible
    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    timer.singleShot(0, this, &ComputeEngine::loop);
}

void ComputeEngine::loop() {

    if(flagDisableProcessing) {
        return;
    }

    double millisCompute;   // time in nanoseconds taken by computation
    double millisLastFrame; // time in nanoseconds since last frame

    // compute time since last frame, with exception if this is the first frame
    if(firstFrame) {
        firstFrame = false;
        millisLastFrame = 1.0 / frequency * 1000.0;
    } else {
        millisLastFrame = elapsedTimer.nsecsElapsed() / 1000000.0;
    }

    // restart the timer to measure frame time and compute time
    elapsedTimer.restart();
    elapsedTimer.start();

    // do the computation
    for(QList<QSharedPointer<Generator>>::iterator it = generators->begin(); it != generators->end(); it++) {
        // do the actual computation
        (*it)->computeOutput(1.0 / frequency);
    }

    // write to outputMonitor
    for(QList<QSharedPointer<Generator>>::iterator it = generators->begin(); it != generators->end(); it++) {
        double outputMonitor = 0;
        for(int i = 0; i < (*it)->getOutputSize(); i++) {
            outputMonitor += (*it)->readOutput(i);
        }
        if(flagDummyOutputMonitor) {
            // random output
            outputMonitor = randomUniform(randomGenerator);
        } else {
            // dumb averaging
            outputMonitor /= (*it)->getOutputSize();
        }
        (*it)->writeOutputMonitor(outputMonitor);
    }

    // send output messages to osc engine
    for(QList<QSharedPointer<Generator>>::iterator it = generators->begin(); it != generators->end(); it++) {
        QList<QVariant> outputs;
        for(int i = 0; i < (*it)->getOutputSize(); i++) {
            double value = flagDummyOutputMonitor ? randomUniform(randomGenerator) : (*it)->readOutput(i);
            if(flagCastOutputToFloat) {
                outputs.append((float)value);
            } else {
                outputs.append(value);
            }
        }
        emit sendOscData((*it)->getId(), outputs);
    }

    // measure the time used to do the computation
    millisCompute = elapsedTimer.nsecsElapsed() / 1000000.0;

    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    timer.singleShot((int) std::min<double>(1.0 / frequency * 1000.0, std::max<double>(1.0 / frequency * 1000.0 - millisCompute, 0)), this, &ComputeEngine::loop);

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "loop (ComputeEngine):\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\trefresh (ms) = " << millisLastFrame << "\tcompute (ms) = " << millisCompute;
    }
}
