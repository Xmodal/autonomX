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
#include <QThread>
#include <QDebug>
#include <time.h>

#include "WolframCA.h"

WolframCA::WolframCA(int id) : Generator(id, "WolframCA", "WCA", "Wolfram CA description"){
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (WolframCA):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    initialize();

}

WolframCA::~WolframCA()
{

}

void WolframCA::initialize(){

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "initialize:\t\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // re-init algorithm here

    // resize cells vector for current lattice size
    cells.resize(latticeHeight * latticeWidth);

    // allocate memory for cell values
//    cellValues = new double[latticeHeight * latticeWidth];
//    for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
//        cellValues[i] = new double[latticeHeight * latticeWidth];
//    }

    // initialize cell values
    for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
                cells[i] = 0;
            }

    // set last generation to the last available row in lattice -> ensures looping at end of lattice
    lastGeneration = latticeHeight;

    // reset iterations and generations
    iterationNumber = 1;
    currentGeneration = 0;

}

void WolframCA::resetParameters()
{
    // reset params here
    initialize();
}

void WolframCA::computeIteration(double deltaTime)
{
    // execute WolframCA here

    ///// line by line generation implementation /////

    // every 1000 iterations, currentGeneration increments and iterationNumber resets
    if(iterationNumber % 100 == 0) {
        currentGeneration++;
        iterationNumber = 0;
    }

    // if last generation has been passed, currentGeneration resets so lattice can begin writing from top
    if(currentGeneration == lastGeneration) {
        currentGeneration = 0;

        // reset all cell values to 0
        for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
            cells[i] = 0;
        }
    }

    // set values of cells, brighter for each generation
    for(int i = 0; i < latticeWidth; ++i) {
        cells[currentGeneration * latticeWidth + i] = currentGeneration / (double)latticeHeight;
    }

    // every iteration, iterationNumber increments
    iterationNumber++;
}

double WolframCA::getLatticeValue(int x, int y)
{
    int index = x % latticeWidth + y * latticeWidth;
    return cells[index];
}

void WolframCA::writeLatticeValue(int x, int y, double value)
{
    // write values to lattice
    int index = x % latticeWidth + y * latticeWidth;
    cells[index] = value;
}

int WolframCA::getRule() {
    return rule;
}

void WolframCA::writeRule(int rule)
{
    this->rule = rule;
    // make sure you follow this signal structure when you write a property!
    emit ruleChanged(rule);
    emit valueChanged("rule", rule);
}

double WolframCA::getTimeScale() const {
    return this->timeScale;
}

void WolframCA::writeTimeScale(double timeScale) {
    if(this->timeScale == timeScale)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeTimeScale:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->timeScale = timeScale;
    emit valueChanged("timeScale", QVariant(timeScale));
    emit timeScaleChanged(timeScale);
}

//double WolframCA::getCellValue() {
//}

//void WolframCA::setCellValue(double value) {
//    this->cellValue = value;
//}
