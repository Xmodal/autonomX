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
#include "algorithm"
#include "iostream"
#include "chrono"
#include <QDebug>
#include <QThread>

ComputeEngine::ComputeEngine(QList<QSharedPointer<Generator>> generators) {
    this->generators = generators;
}

ComputeEngine::~ComputeEngine() {
    std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now().time_since_epoch()
    );
    std::cout << "destructor (ComputeEngine):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << std::endl;
}

void ComputeEngine::start() {
    // this is called from an external thread initially and allows this object's thread to pick up the loop statement through its event queue
    // having a singleshot timer with time zero executes as soon as possible
    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    timer.singleShot(0, this, &ComputeEngine::loop);
}

void ComputeEngine::loop() {

    if(flagDebug) {
        /*
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );
        std::cout << std::endl;
        std::cout << "ComputeStart:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << std::endl;
        */
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
    for(QList<QSharedPointer<Generator>>::iterator it = generators.begin(); it != generators.end(); it++) {
        // do the actual computation
        (*it)->computeOutput(1.0 / frequency);
        // update the value of the output monitor
        double outputMonitor = 0;
        for(int i = 0; i < (*it)->getOutputSize(); i++) {
            outputMonitor += (*it)->readOutput(i);
        }
        // dumb averaging
        outputMonitor /= (*it)->getOutputSize();
        // maybe doing this here is bad? does the overhead of the signaling slow down the loop?
        (*it)->writeOutputMonitor(outputMonitor);
    }

    // measure the time used to do the computation
    millisCompute = elapsedTimer.nsecsElapsed() / 1000000.0;

    if(flagDebug) {
        /*
        qDebug() << "refresh interval: " << millisLastFrame;
        qDebug() << "compute time:     " << millisCompute;
        */

        /*
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );
        std::cout << "ComputeEnd:\t\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << std::endl;
        */
    }

    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    timer.singleShot((int) std::min<double>(1.0 / frequency * 1000.0, std::max<double>(1.0 / frequency * 1000.0 - millisCompute, 0)), this, &ComputeEngine::loop);
}
