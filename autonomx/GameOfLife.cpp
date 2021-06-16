#include <chrono>
#include <QThread>
#include <QDebug>
#include <time.h>

#include "GameOfLife.h"

GameOfLife::GameOfLife(int id, GeneratorMeta * meta) : Generator(id, meta)
{
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (WolframCA):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    initialize();
}

GameOfLife::~GameOfLife()
{

}

void GameOfLife::initialize()
{
    // re-initialize algorithm here

    // resize cells vector for current lattice size
    cells.resize(latticeHeight * latticeWidth);

    // initialize cell values
    for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
               double magic = (float) rand()/RAND_MAX;
               if (magic>0.7)
                   cells[i]=1;
               else
                  cells[i]=0;
            }

    // start the iterations
    iterationNumber = 1;
}

void GameOfLife::computeIteration(double deltaTime)
{
    // compute iteration here
    if (iterationNumber%20==0){
    for (int i=1; i<latticeWidth-1;i++){
        for (int j=1;j<latticeHeight-1;j++){

            // find index of the main cell, the cell just above it, and cell just below it; once we do that we can easily find the cells adjacent to those cells
               int index = i % latticeWidth + j * latticeWidth;
               int index_above = (i-1) % latticeWidth + j * latticeWidth;
               int index_below = (i+1) % latticeWidth + j * latticeWidth;

               // finding the total value of all the neighbors near to the main cell
//               int neighbors = cells[index_above-1] + cells[index_above] + cells[index_above+1]
//                       + cells[index-1] + cells[index+1]
//                       + cells[index_below-1] + cells[index_below] + cells[index_below+1];

               int neighbors = cells[index - latticeWidth - 1] + cells[index - latticeWidth] + cells[index - latticeWidth + 1]
                       + cells[index - 1] + cells[index] + cells[index + 1]
                       + cells[index + latticeWidth - 1] + cells[index + latticeWidth] + cells[index + latticeWidth + 1];

               // declare the cell alive or dead based on the rule below
                if      ((cells[index] == 1) && (neighbors <  2)) cells[index] = 0;
                //TO DO: Here neighbors should be 3 infront of 4 in the just below line. However, changed to 4 to make the GOL look decent. Artistic Lisence. Can discuss.
                else if ((cells[index] == 1) && (neighbors >  4)) cells[index] = 0;
                else if ((cells[index] == 0) && (neighbors == 3)) cells[index] = 1;
        }

    }
    }
    iterationNumber++;
}


double GameOfLife::getLatticeValue(int x, int y)
{
    int index = x % latticeWidth + y * latticeWidth;
//  std::cout << "Index = " + index;
    return cells[index];
}

void GameOfLife::writeLatticeValue(int x, int y, double value)
{
    // write values to lattice
    int index = x % latticeWidth + y * latticeWidth;
    cells[index] = value;
}

int GameOfLife::getRule()
{
    return rule;
}

void GameOfLife::writeRule(int rule)
{ if(this->rule == rule)
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
