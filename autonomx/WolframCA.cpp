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

}

void WolframCA::resetParameters()
{
    // reset params here
    initialize();
}

void WolframCA::computeIteration(double deltaTime)
{
    // execute CA here

    srand( (unsigned)time( NULL ) );

    for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
        for(int j = 0; j < (latticeHeight * latticeWidth); ++j) {
            cells[i] = (float) rand()/RAND_MAX;
        }



//        cells[i] = sigmoid(deltaTime);
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
