#include <chrono>
#include <QThread>
#include <QDebug>
#include <time.h>
#include <math.h>

#include "GameOfLife.h"

GameOfLife::GameOfLife(int id, GeneratorMeta * meta) : Generator(id, meta)
{
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (WolframCA):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    timeScale = 95;
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
    //maintain a temporary cell to overwrite
    temp_cells.resize(latticeHeight * latticeWidth);

    // draw pattern when initializing
    drawPattern(GOLPattern);

    // start the iterations
    iterationNumber = 1;
}


void GameOfLife::drawPattern(GOLPatternType type) {

    switch (type) {
        // Random
        case GOLPatternType::Random: {
            // initialize cell values
            for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
            double magic = (float) rand()/RAND_MAX;

            if (magic>0.5)
                cells[i]=1;
            else
                cells[i]=0;
            }
            break;
        }

        // Glider
        case GOLPatternType::Glider: {

            // initialize all cell values to zero initially
            for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
                cells[i] = 0;
            }
            // assign cell values to 1 to make a glider in the middle of the lattice
            int indexTop = (latticeWidth/2) % latticeWidth + latticeHeight/2  * latticeWidth;
            int indexMiddle = indexTop+latticeWidth;
            int indexBottom = indexMiddle+latticeWidth;
            cells[indexTop]=1; cells[indexTop+1]=1; cells[indexTop+2]=1;
            cells[indexMiddle]=1;
            cells[indexBottom+1]=1;

            break;
        }

        // SpaceShip
        case GOLPatternType::SpaceShip: {

            // initialize all cell values to zero initially
            for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
                cells[i] = 0;
            }

            // assign cell values to 1 to make a SpaceShip in the middle of the lattice
            int indexTop = (latticeWidth/2) % latticeWidth + latticeHeight/2  * latticeWidth;
            int indexMiddle = indexTop+latticeWidth;
            int indexBottom = indexMiddle+latticeWidth;
            int indexBottom2 = indexBottom+latticeWidth;
            int indexBottom3 = indexBottom2+latticeWidth;

            cells[indexTop+1]=1; cells[indexTop+2]=1; cells[indexTop+3]=1;
            cells[indexMiddle]=1; cells[indexMiddle+3]=1;
            cells[indexBottom+3]=1;
            cells[indexBottom2+3]=1;
            cells[indexBottom3]=1; cells[indexBottom3+2]=1;
            break;
        }

        // RPentoMino
        case GOLPatternType::RPentoMino: {

            // initialize all cell values to zero initially
            for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
                cells[i] = 0;
            }
            // assign cell values to 1 to make a RPentoMino in the middle of the lattice
            int indexTop = (latticeWidth/2) % latticeWidth + latticeHeight/2  * latticeWidth;
            int indexMiddle = indexTop+latticeWidth;
            int indexBottom = indexMiddle+latticeWidth;
            cells[indexTop+1]=1; cells[indexTop+2]=1;
            cells[indexMiddle]=1; cells[indexMiddle+1]=1;
            cells[indexBottom+1]=1;

            break;
        }

        // Pentadecathlon
        case GOLPatternType::Pentadecathlon: {
            for(int i = 0; i < (latticeHeight * latticeWidth); ++i) {
                cells[i] = 0;
            }

            // assign cell values to 1 to make a Pentadecathlon in the middle of the lattice
            //index top here would refer to the leftmost cell of the middle rectangle here
            int indexTop = (latticeWidth/2) % latticeWidth + latticeHeight/2  * latticeWidth;
            int indexMiddle = indexTop+latticeWidth;
            int indexBottom = indexMiddle+latticeWidth;
            cells[indexTop] = 1; cells[indexTop+1]=1; cells[indexTop+3]=1;cells[indexTop+6]=1;cells[indexTop] = 1; cells[indexTop-1]=1; cells[indexTop-3]=1;cells[indexTop-6]=1;
            cells[indexMiddle]=1; cells[indexMiddle+1]=1; cells[indexMiddle+3]=1; cells[indexMiddle+4]=1; cells[indexMiddle+5]=1; cells[indexMiddle+6]=1;cells[indexMiddle]=1; cells[indexMiddle-1]=1; cells[indexMiddle-3]=1; cells[indexMiddle-4]=1; cells[indexMiddle-5]=1; cells[indexMiddle-6]=1;
            cells[indexBottom] = 1; cells[indexBottom+1]=1; cells[indexBottom+3]=1;cells[indexBottom+6]=1;cells[indexBottom] = 1; cells[indexBottom-1]=1; cells[indexBottom-3]=1;cells[indexBottom-6]=1;
            break;
        }
    }
}

void GameOfLife::computeIteration(double deltaTime)
{
    temp_cells = cells;

    // compute iteration here
    if (timeScale > 0 && iterationNumber > (int)(1 / (pow(timeScale / 100, 2)))) {
        iterationNumber = 0;
        for (int i = 1; i < latticeHeight - 1; i++) {
            for (int j = 1; j < latticeWidth - 1; j++) {

                // find index of the main cell, the cell just above it, and cell just below it; once we do that we can easily find the cells adjacent to those cells
                int index = i * latticeWidth + j;
                int index_above = index-latticeWidth;
                int index_below = index+latticeWidth;

                // finding the total value of all the neighbors near to the main cell
                neighbors = 0; //initializing a default value
                // if the cells are on the extreme left of the lattice consider the cells on the extreme right as their neighbors :ignoring the 4 corner cells
                if (j==0 && i!=0 && i!=latticeHeight-1){
                    neighbors = cells[index_above+latticeWidth-1] + cells[index_above] + cells[index_above+1]
                        + cells[index+latticeWidth-1] + cells[index+1]
                        + cells[index_below+latticeWidth-1] + cells[index_below] + cells[index_below+1];
                }
                // if the cells are on the extreme right of the lattice consider the cells on the extreme left as their neighbors:ignoring the 4 corner cells
                else if (j==latticeWidth-1 && i!=0 && i!=latticeHeight-1){
                    neighbors = cells[index_above-1] + cells[index_above] + cells[index_above-latticeWidth+1]
                    + cells[index-1] + cells[index-latticeWidth+1]
                    + cells[index_below-1] + cells[index_below] + cells[index_below-latticeWidth+1];
                }
                //if the cells are on the upper most layer consider the cells on the extreme down as their neighbors:ignoring the 4 corner cells
                else if (i==0 && j!=0 && j!=latticeWidth-1){
                    index_above = index+(latticeWidth)*(latticeHeight-1);
                    neighbors = cells[index_above-1] + cells[index_above] + cells[index_above-latticeWidth+1]
                    + cells[index-1] + cells[index-latticeWidth+1]
                    + cells[index_below-1] + cells[index_below] + cells[index_below-latticeWidth+1];
                }
                 //if the cells are on the last layer consider the cells on the extreme upper side of the lattice as their neighbors:ignoring the 4 corner cells
                else if (i==latticeHeight-1 && j!=0 && j!=latticeWidth-1){
                    index_below = index-(latticeWidth)*(latticeHeight-1);
                    neighbors = cells[index_above-1] + cells[index_above] + cells[index_above-latticeWidth+1]
                    + cells[index-1] + cells[index-latticeWidth+1]
                    + cells[index_below-1] + cells[index_below] + cells[index_below-latticeWidth+1];
                }
                //for any other cell on the lattice
                else{
                    neighbors = cells[index_above-1] + cells[index_above] + cells[index_above+1]
                            + cells[index-1] + cells[index+1]
                            + cells[index_below-1] + cells[index_below] + cells[index_below+1];
                }


                // declare the cell alive or dead based on the rules below - check Wikipedia for these rules
                if ((cells[index] == 1) && (neighbors < 2)) temp_cells[index] = 0;
                else if ((cells[index] == 1) && (neighbors >  3)) temp_cells[index] = 0;
                else if ((cells[index] == 0) && (neighbors == 3)) temp_cells[index] = 1;
                else temp_cells[index] = cells[index];

            }
        }
    }

    //TO DO: write a logic to reinitialize if all the cells are zero or may be if there is no change in pattern
    //check if all the cells are zero to re-iterate

    /* int check = 0;
    for (int i = 0; i < latticeWidth * latticeHeight; i++) {
        if (temp_cells[i] > 0) {
            check = 1;
            qDebug()<<"cell no"<<i;
        }
    }

    //check if all the cells are zero
    if (check == 0)
        initialize();
    */

    iterationNumber++;
    cells = temp_cells;
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

GOLPatternType GameOfLife::getGOLPattern() const {
    return GOLPattern;
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

void GameOfLife::writeGOLPattern(GOLPatternType GOLPattern){
    if(this->GOLPattern == GOLPattern)
           return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                   std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "Game of Life Pattern:\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
    //do the change
    this->GOLPattern = GOLPattern;
    //reinitialize
    initialize();
    // make sure you follow this signal structure when you write a property!
    emit GOLPatternChanged(GOLPattern);
    emit valueChanged("GOLPattern", QVariant(GOLPattern));
}
