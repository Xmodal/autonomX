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

#include "WolframCA.h"

#include <time.h>

#include <QDebug>
#include <QThread>
#include <chrono>
#include <random>

WolframCA::WolframCA(int id, GeneratorMeta* meta) : Generator(id, meta) {
    if (flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch());

        qDebug() << "constructor (WolframCA):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    speed = 50;
    initialize();
}

WolframCA::~WolframCA() {
}

void WolframCA::initialize() {
    if (flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch());

        qDebug() << "initialize:\t\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // resize cells vector for current lattice size
    cells.resize(latticeHeight * latticeWidth);

    // to store the cells in the next generation
    nextGenCells.resize(latticeWidth);

    // reset all cells to 0 / black
    for (int i = 0; i < (latticeHeight * latticeWidth); i++) {
        cells[i] = 0;
        if (i < latticeWidth) nextGenCells[i] = 0;
    }

    // a var to store a random number
    double magic;

    if (!flag_randomSeed) {
        cells[latticeWidth * latticeHeight - 1 - latticeWidth / 2] = 1;  // middle cell on last line of the lattice is the starting seed
    }
    // if random starting seed is selected by user
    else {
        for (int i = 0; i < (latticeHeight * latticeWidth); i++) {  //initialize a random cell as starting cell
            if (i <= latticeWidth * latticeHeight - latticeWidth)
                cells[i] = 0;
            else {
                magic = (float)rand() / RAND_MAX;  // generate a random number magic b/w 0-1 and initialize to 1 if magic>0.5 o/w magic -> 0
                if (magic > 0.5)
                    cells[i] = 1;
                else
                    cells[i] = 0;
            }
        }
    }

    lastGeneration = latticeHeight;
    // current generation represents the generation number of the automata
    currentGeneration = 0;
    iterationNumber = 1;

    //generate the rule set array based on the defined rule
    int r = getRule();
    generate(r);
}

void WolframCA::computeIteration(double deltaTime) {
    totalTime += deltaTime * (speed / 100);

    //set ruleset using the determined rule after checking if the rule has changed by the user
    int r = getRule();

    // change rule set if changed by the user
    if (iterationNumber >= 1 && (prev_rule != rule)) {
        generate(r);
    }

    //check if randomness radio button is selected or not
    if (getFlagRandomSeed())
        flag_randomSeed = true;
    else
        flag_randomSeed = false;

    // every 1000 iterations, currentGeneration increments and iterationNumber resets
    if (totalTime >= 1.0 / 60.0) {
        totalTime -= (1.0 / 60.0);
        currentGeneration++;
        iterationNumber = 1;
        //qDebug()<<"Current Generation is "<<currentGeneration;
    }

    //move the drone shot
    if (iterationNumber == 1 && currentGeneration > 0) {
        for (int i = 0; i < latticeWidth; i++) {
            //if the cell is the leftmost cell
            if (i == 0) {
                int left = cells[latticeWidth * latticeHeight - 1];
                int middle = cells[latticeWidth * latticeHeight - latticeWidth + i];
                int right = cells[latticeWidth * latticeHeight - latticeWidth + i + 1];
                nextGenCells[i] = findCellValue(left, middle, right);
            }
            //if the cell is the rightmost cell
            else if (i == latticeWidth - 1) {
                int left = cells[latticeWidth * latticeHeight - latticeWidth + i - 1];
                int middle = cells[latticeWidth * latticeHeight - latticeWidth + i];
                int right = cells[latticeWidth * latticeHeight - latticeWidth];
                nextGenCells[i] = findCellValue(left, middle, right);
            } else {
                int left = cells[latticeWidth * latticeHeight - latticeWidth + i - 1];
                int middle = cells[latticeWidth * latticeHeight - latticeWidth + i];
                int right = cells[latticeWidth * latticeHeight - latticeWidth + i + 1];
                nextGenCells[i] = findCellValue(left, middle, right);
            }
        }
        moveDrone(nextGenCells);  //shift the entire automata one line above
    }

    //store the rule to check next time if the rule has changed
    prev_rule = rule;

    // every iteration, iterationNumber increments
    iterationNumber++;
}

void WolframCA::moveDrone(std::vector<double> nextGencells) {
    //shift cells starting from the top of the lattice till n-1 line - one line above
    for (int i = latticeWidth; i < latticeWidth * latticeHeight; i++) {
        cells[i - latticeWidth] = cells[i];
    }
    //create the new generation on the last line of the lattice
    int iter = 0;
    for (int j = latticeWidth * latticeHeight - latticeWidth; j < latticeWidth * latticeHeight; j++) {
        cells[j] = nextGencells[iter];
        iter++;
    }
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
    return ruleset[0];
}

void WolframCA::generate(int r) {
    //Grabs the binary value from the int and inserts into array
    for (int i = sizeof(r) * CHAR_BIT; i--;) {
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
    return rule;
}

void WolframCA::writeRule(int rule) {
    if (this->rule == rule)
        return;

    if (flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch());

        qDebug() << "Rule:\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
    this->rule = rule;
    // make sure you follow this signal structure when you write a property!
    emit ruleChanged(rule);
    emit valueChanged("rule", rule);
}

bool WolframCA::getFlagRandomSeed() const {
    return this->flag_randomSeed;
    qDebug() << "flag_randomSeed";
}

void WolframCA::writeFlagRandomSeed(bool flag_randomSeed) {
    if (this->flag_randomSeed == flag_randomSeed)
        return;

    if (flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch());

        qDebug() << "writeFlagRandSeed:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->flag_randomSeed = flag_randomSeed;
    emit valueChanged("flag_randomSeed", QVariant(flag_randomSeed));
    emit flagRandomSeedChanged(flag_randomSeed);
}
