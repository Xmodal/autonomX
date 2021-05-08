#include <chrono>
#include <QThread>
#include <QDebug>
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
    cellValues = new double*[latticeHeight * latticeWidth];
    for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
        cellValues[i] = new double[latticeHeight * latticeWidth];
    }

    // initialize cell values
    for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
        for(int j = 0; j < (latticeHeight * latticeWidth); ++j) {
            if (j % 2 == 0 ) {
                cellValues[i][j] = j * 5.0;
            } else {
                cellValues[i][j] = j * 10.0;
            }
        }
    }

}

void WolframCA::resetParameters()
{
    // reset params here
}

void WolframCA::computeIteration(double deltaTime)
{
    // execute CA here
    deltaTime *= timeScale;

    initialize();
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

int WolframCA::getRule()
{
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
