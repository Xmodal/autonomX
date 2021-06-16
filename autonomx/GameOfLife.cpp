#include <chrono>
#include <QThread>
#include <QDebug>
#include <time.h>

#include "GameOfLife.h"

GameOfLife::GameOfLife(int id) : Generator(id, "Game of Life", "GOL", "Game of Life description")
{
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (GAMEOFLIFE):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
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
               if (magic>0.9)
                   cells[i]=1;
               else
                  cells[i]=0;
    }

    // start the iterations
    iterationNumber = 1;
}

void GameOfLife::computeIteration(double deltaTime)
{
    qDebug() << "computeIteration";
    // compute iteration here
    // every 20 ticks of engine's internal clock
    if (iterationNumber%20==0){
        // testing implementations

//        // for every cell on the lattice, do the following
//        for (int i=1; i<latticeWidth-1;i++){
//            for (int j=1;j<latticeHeight-1;j++){
//                // find index of the main cell, the cell just above it, and cell just below it; once we do that we can easily find the cells adjacent to those cells
//                int index = i % latticeWidth + j * latticeWidth;
//                int index_above = (i-1) % latticeWidth + j * latticeWidth;
//                int index_below = (i+1) % latticeWidth + j * latticeWidth;
//                // finding the total value of all the neighbors near to the main cell
//                int neighbors = cells[index_above-1] + cells[index_above] + cells[index_above+1]
//                    + cells[index-1] + cells[index+1]
//                    + cells[index_below-1] + cells[index_below] + cells[index_below+1];
//                // declare the cell alive or bad based on the rule below
//                if((cells[index] == 1) && (neighbors <  2)) cells[index] = 0;
//                    //TO DO: Here neighbors should be 3 infront of 4 in the just below line. However, changed to 4 to make the GOL look decent. Artistic Lisence. Can discuss.
//                else if ((cells[index] == 1) && (neighbors >  4)) cells[index] = 0;
//                else if ((cells[index] == 0) && (neighbors == 3)) cells[index] = 1;
//            }

//        }
//        } else {

        // ursula version

        int indexGlobal = 0;

        for(int i = 0; i < latticeHeight; i++) {
            for(int j = 0; j < latticeWidth; j++) {

                // cell values of a neighbourhood
                int topLeft = 0, topCenter = 0, topRight = 0,
                        middleLeft = 0, middleCenter = 0, middleRight = 0,
                        bottomLeft = 0, bottomCenter = 0, bottomRight = 0;
                // cell location barrier checks
                bool topRow = true;
                bool leftColumn = true;
                bool rightColumn = true;
                bool bottomRow = true;

                // index = cell index number
                int index = (i * latticeHeight) + j;
                qDebug() << "index = " << index;
                // if index is not on top row, then we can calculate cells above it
                if(index >= latticeWidth) {
                    qDebug() << "cell is not on top row";
                    topRow = false;
                } else {
                    qDebug() << "cell is on top row";
                }
                // if index is not on bottom row, then we can calculate cells below it
                if(index <= ((latticeHeight * latticeWidth) - latticeWidth)) {
                    qDebug() << "cell is not on bottom row";
                    bottomRow = false;
                } else {
                    qDebug() << "cell is on bottom row";
                }
                // if index is not in left-most column, then we can calculate cells left of it
                if(index % latticeHeight != 0) {
                    qDebug() << "cell is not in left-most column";
                    leftColumn = false;
                } else {
                    qDebug() << "cell is in left-most column";
                }
                // if index is not in right-most column, then we can calculate cells right of it
                if(index % latticeHeight != 9) {
                    qDebug() << "cell is not in right-most column";
                    rightColumn = false;
                } else {
                    qDebug() << "cell is in right-most column";
                }

                // calculates top row of neighbourhood, if possible
                if(!topRow) {
                    topCenter = cells[index - latticeWidth];
                    if(!leftColumn) {
                        topLeft = cells[index - latticeWidth - 1];
                    }
                    if(!rightColumn) {
                        topRight = cells[index - latticeWidth + 1];
                    }
                }
                // calculates bottom row of neightbourhood, if possible
                if(!bottomRow) {
                    bottomCenter = cells[index + latticeWidth];
                    if(!leftColumn) {
                        bottomLeft = cells[index + latticeWidth - 1];
                    }
                    if(!rightColumn) {
                        bottomRight = cells[index + latticeWidth + 1];
                    }
                }
                // calculates middle row of neighbourhood (where possible)
                middleCenter = cells[index];
                if(!leftColumn) {
                    middleLeft = cells[index - 1];
                }
                if(!rightColumn) {
                    middleRight = cells[index + 1];
                }

              int neighbourhood = topLeft + topCenter + topRight
                      + middleLeft + middleRight
                      + bottomLeft + bottomCenter + bottomRight;

              qDebug() << "neighbourhood: " << neighbourhood;

              // declare the cell alive or bad based on the rule below
//              if((cells[index] == 1) && (neighbourhood <  2)) cells[index] = 0;
//              else if ((cells[index] == 1) && (neighbourhood >=  4)) cells[index] = 0;
//              else if ((cells[index] == 0) && (neighbourhood == 3)) cells[index] = 1;

              // if cell isn't dead
              if(cells[index] == 1) {
                  if(neighbourhood <= 1) {
                      cells[index] = 0;
                  } else if(neighbourhood >= 4) {
                      cells[index] = 0;
                  } else {
                      cells[index] = 1;
                  }
              // if cell is dead
              } else {
                  if(neighbourhood == 3) {
                      cells[index] == 1;
                  } else {
                      cells[index] = 0;
                  }
              }

//              indexGlobal++;
//              cells[indexGlobal] = 0;
            }
        }
//    }
    }
    iterationNumber++;
}

void GameOfLife::resetParameters()
{
    initialize();
    // reset parameters here
    // this also implies emitting all property signals for QML to notice & change GUI consequently
    // we will eventually discuss an ideal implementation strategy for this action
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
{
    this->rule = rule;
    // make sure you follow this signal structure when you write a property!
    emit ruleChanged(rule);
    emit valueChanged("rule", rule);
}
