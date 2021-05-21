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

    // initialize cell values
    for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
        cells[i]=0;
    }
    cells[latticeWidth/2]=1;

    lastGeneration = latticeHeight;
    iterationNumber = 1;

    //generate the rule set array based on the defined rule
    int r = getRule();
    generate(r);
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

    //set ruleset using the determined rule after checking if the rule has changed by the user
    int r = getRule();
    if (iterationNumber>=1 && (prev_rule!=rule))
    generate(r);

    // every 1000 iterations, currentGeneration increments and iterationNumber resets
    if(iterationNumber % 50 == 0) {
        currentGeneration++;
        iterationNumber = 0;
    }

    // if last generation has been passed, currentGeneration resets so lattice can begin writing from top
    if(currentGeneration == lastGeneration) {
        currentGeneration = 1;

        //continuing the lattice from the top as the cells have fallen to the bottom
        for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
            if (i==0){
                // to restart the cells from the top but with a continuing pattern from below; hence intializing the topmost layer once again
                for (int j = 1 ; j < latticeWidth-1; ++j){
                    cells[j] = cells[(latticeHeight) * (latticeHeight-1) + j];
                }
            }
            if (i>latticeWidth)
                cells[i] = 0;
        }
       // cells[latticeWidth/2]=1;
    }

    //write the main logic here to get values of next generation

    for(int i = 1; i < latticeWidth-1; ++i) {
        int left = cells[(currentGeneration-1) * latticeWidth + i - 1];
        int right = cells[(currentGeneration-1) * latticeWidth + i + 1];
        int middle = cells[(currentGeneration-1) * latticeWidth + i];
        cells[(currentGeneration) * latticeWidth + i] = findCellValue(left,middle,right);
        //qDebug() << findCellValue(left,middle,right);
        //cells[currentGeneration * latticeWidth + i] =  i*0.5; //currentGeneration / (double)latticeHeight;
    }

  /*  // set values of cells, brighter for each generation
    for(int i = 0; i < latticeWidth; ++i) {
        cells[currentGeneration * latticeWidth + i] =  i*0.5; //currentGeneration / (double)latticeHeight;
    }*/

    //store the rule to check next time if the rule has changed
    prev_rule = rule;

    // every iteration, iterationNumber increments
    iterationNumber++;
}

int WolframCA::findCellValue(int a, int b, int c) {
    if (a == 1 && b == 1 && c == 1) {
        return ruleset[7];
    }
    if (a == 1 && b == 1 && c == 0) {
        return ruleset[6];
    }
    if (a == 1 && b == 0 && c == 1) {
        return ruleset[5];
    }
    if (a == 1 && b == 0 && c == 0) {
        return ruleset[4];
    }
    if (a == 0 && b == 1 && c == 1) {
        return ruleset[3];
    }
    if (a == 0 && b == 1 && c == 0) {
        return ruleset[2];
    }
    if (a == 0 && b == 0 && c == 1) {
        return ruleset[1];
    }
    //else (a == 0 && b == 0 && c == 0) {
        return ruleset[0];
    //}
}

void WolframCA::generate(int r){

    //Grabs the binary value from the int and inserts into array
       for (int i = sizeof(r) * CHAR_BIT; i--; )
       {
           if (i < 8) {
               char temp = ('0' + ((r >> i) & 1));
               int x = temp - '0';
               ruleset[i] = x;
           }
       }
       //to check the update rulesets
       //for (int j=0;j<8;j++)
       //qDebug() << "The value of new ruleset is is  "<< ruleset[j];
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
    //qDebug() <<"New Rule is"<< rule;
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
