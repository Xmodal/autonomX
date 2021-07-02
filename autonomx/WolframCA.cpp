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
#include <random>

#include "WolframCA.h"

WolframCA::WolframCA(int id, GeneratorMeta * meta) : Generator(id, meta){
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch());

        qDebug() << "constructor (WolframCA):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    initialize();

}

WolframCA::~WolframCA() {

}

void WolframCA::initialize() {

if(flagDebug) {
    std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
    std::chrono::system_clock::now().time_since_epoch());

   qDebug() << "initialize:\t\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
}

// resize cells vector for current lattice size
cells.resize(latticeHeight * latticeWidth);

// reset all cells to 0 / black
for(int i = 0; i < (latticeHeight * latticeWidth); i++) {
 cells[i] = 0;
}

// a var to store a random number
double magic;

if (!flag_randSeed) {
   cells[latticeWidth / 2] = 1;    // middle cell on first line is starting seed
}
// if random starting seed is selected by user
else {
       for(int i = 0; i < (latticeHeight * latticeWidth); i++) {  //initialize a random cell as starting cell
       //    for(int i = 0; i < latticeWidth; i++) {  // initialize a random cell as starting cell
           if (i >= latticeWidth)
               cells[i] = 0;
           else {
               magic = (float) rand() / RAND_MAX;  // generate a random number magic b/w 0-1 and initialize to 1 if magic>0.5 o/w magic -> 0
               if (magic > 0.5)
                   cells[i] = 1;
               else
                  cells[i] = 0;
           }
       }
}

   // TODO:
   // fix this random seed bug
   // temporary fix below
   // selects one random cell from the first row to be the random seed
   // ensures that at least one cell is seleted as seed
   // otherwise, as the random seed approaches 1, the likelihood that no cell will be initialized increases
   // at max value (1) it is 100% certain that no seed will be selected
   // no seed selected = a blank lattice will be generated -> essentially breaks the generator until reset

   /*if(seedChosen == false) {
       magic = (float) rand() / RAND_MAX;
       qDebug() << "no cell was selected normally, temp fix triggered." << endl << "random cell selected for seed: " << (int)(magic * latticeWidth);
       cells[(int)(magic * latticeWidth)] = 1;
   }*/


   lastGeneration = latticeHeight;
   currentGeneration = 0;
   iterationNumber = 1;

   //generate the rule set array based on the defined rule
   int r = getRule();
   generate(r);
}

void WolframCA::computeIteration(double deltaTime) {
    //set ruleset using the determined rule after checking if the rule has changed by the user
    int r = getRule();
    if (iterationNumber >= 1 && (prev_rule!=rule)) {
        generate(r);
    }

    //check if randomness radio button is selected or not
    if(getFlagRandSeed())
        flag_randSeed=true;
    else
        flag_randSeed=false;

    int timeScale = getTimeScale();

    // every 1000 iterations, currentGeneration increments and iterationNumber resets
    if(iterationNumber % (100-timeScale+1) == 0) {
        currentGeneration++;
        iterationNumber = 1;
    }

    // if last generation has been passed, currentGeneration resets so lattice can begin writing from top
    if(currentGeneration == lastGeneration) {
        currentGeneration = 1;

        //continuing the lattice from the top as the cells have fallen to the bottom
        for(int i = 0; i < (latticeHeight * latticeWidth); i++) {
            if (i == 0){
                // to restart the cells from the top but with a continuing pattern from below; hence intializing the topmost layer once again
                for (int j = 1 ; j < latticeWidth - 1; j++){
                    cells[j] = cells[(latticeHeight) * (latticeHeight - 1) + j];
                }
            }
            if (i > latticeWidth)
                cells[i] = 0;
        }
    }

    //write the main logic here to get values of next generation -- remmeber the current generation starts from 1 hence current generation -1
if(currentGeneration>0){
    for(int i = 0 ; i < latticeWidth; i++) {
        // to check for the leftmost cell
        if (i == 0){
            int left = cells[(currentGeneration - 1) * latticeWidth + latticeWidth - 1];
            int right = cells[(currentGeneration - 1) * latticeWidth + i + 1];
            int middle = cells[(currentGeneration - 1) * latticeWidth + i];
            cells[(currentGeneration) * latticeWidth + i] = findCellValue(left,middle,right);
        }
        // to check for the rightmost cell
        else if (i == latticeWidth - 1){
            int left = cells[(currentGeneration - 1) * latticeWidth + i - 1];
            int right = cells[(currentGeneration - 1) * latticeWidth + i - (latticeWidth - 1)];
            int middle = cells[(currentGeneration - 1) * latticeWidth + i];
            cells[(currentGeneration) * latticeWidth + i] = findCellValue(left,middle,right);
        }
        //any other cell on the lattice
        else{
             int left = cells[(currentGeneration - 1) * latticeWidth + i - 1];
             int right = cells[(currentGeneration - 1) * latticeWidth + i + 1];
             int middle = cells[(currentGeneration - 1) * latticeWidth + i];
             cells[(currentGeneration) * latticeWidth + i] = findCellValue(left,middle,right);
        }
    }
}
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
    // else (a == 0 && b == 0 && c == 0) {
        return ruleset[0];
    // }
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
}

double WolframCA::getLatticeValue(int x, int y) {
    int index = x % latticeWidth + y * latticeWidth;
    return cells[index];
}

void WolframCA::writeLatticeValue(int x, int y, double value) {
    // write values to lattice
    int index = x % latticeWidth + y * latticeWidth;
    cells[index] = value;
}

int WolframCA::getRule() {
    //qDebug() <<"New Rule is"<< rule;
    return rule;
}

void WolframCA::writeRule(int rule) {
    if(this->rule == rule)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "Rule:\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
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

/*double WolframCA::getRandSeed() const {
    return this->randSeed;
    qDebug()<<randSeed;
}

void WolframCA::writeRandSeed(double randSeed) {
    if(this->randSeed == randSeed)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeRandSeed:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
        randSeed=0.5;
    this->randSeed = randSeed;
    emit valueChanged("randSeed", QVariant(randSeed));
    emit randSeedChanged(randSeed);
}*/

bool WolframCA::getFlagRandSeed() const{
    return this->flag_randSeed;
    qDebug()<<"flag_randSeed";
}

void WolframCA::writeFlagRandSeed(bool flag_randSeed){
    if(this->flag_randSeed == flag_randSeed)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeFlagRandSeed:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->flag_randSeed = flag_randSeed;
    emit valueChanged("flag_randSeed", QVariant(flag_randSeed));
    emit flagRandSeedChanged(flag_randSeed);
}
