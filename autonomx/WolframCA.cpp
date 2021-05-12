#include <chrono>
#include <QThread>
#include <QDebug>
#include <time.h>
//#include <iostream>

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

//WolframCA::WolframCA(const WolframCA &wca1)
//{

//}

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

    // set last generation to the last available row in lattice
    lastGeneration = latticeHeight;
    iterationNumber = 1;

}

void WolframCA::resetParameters()
{
    // reset params here
    initialize();
}

void WolframCA::computeIteration(double deltaTime)
{
    // execute CA here

    ///// random float values on lattice version /////
//    srand( (unsigned)time( NULL ) );

//    for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
//            cells[i] = (float) rand()/RAND_MAX;
//        }

    ///// line by line generation implementation /////

    // every iteration, iterationNumber increments
    iterationNumber++;

    // every 1000 iterations, currentGeneration increments and iterationNumber resets
    if(iterationNumber % 100 == 0) {
        currentGeneration++;
        iterationNumber = 0;
    }

    // if last generation has been passed, currentGeneration resets so lattice can begin writing from top
    if(currentGeneration == lastGeneration) {
        currentGeneration = 1;

        // reset all cell values to 0
        for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
            cells[i] = 0;
        }
    }

    // set values of cells, brighter for each generation
    for(int i = 0; i < latticeWidth; ++i) {
        cells[currentGeneration * latticeWidth + i] = currentGeneration / (double)latticeHeight;
    }
}

double WolframCA::getLatticeValue(int x, int y)
{
    int index = x % latticeWidth + y * latticeWidth;
//    std::cout << "Index = " + index;
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

double WolframCA::sigmoid(double value) {
    // https://www.desmos.com/calculator/ikdusaa9yh
    // sigmoid function centered at 0
    // df/dx = 1 at x = 0
    // f(0) = 0
    // f(infinity) = 1
    // f(-infinity) = -1

    return 1.0 / (1.0 + exp(-value));
}

//double WolframCA::getCellValue() {
//}

//void WolframCA::setCellValue(double value) {
//    this->cellValue = value;
//}
