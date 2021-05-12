#include <chrono>
#include <QThread>
#include <QDebug>
#include <time.h>

#include "GameOfLife.h"

GameOfLife::GameOfLife(int id) : Generator(id, "Game of Life", "GOL", "Game of Life description")
{

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
                cells[i] = false;
            }

    // set last generation
    lastGeneration = latticeHeight;
    iterationNumber = 1;
}

void GameOfLife::computeIteration(double deltaTime)
{
    // compute iteration here
}

void GameOfLife::resetParameters()
{
    // reset parameters here
    // this also implies emitting all property signals for QML to notice & change GUI consequently
    // we will eventually discuss an ideal implementation strategy for this action
}

double GameOfLife::getLatticeValue(int x, int y)
{
    // every value is 0 for now :-)
    return 0.0;
}

void GameOfLife::writeLatticeValue(int x, int y, double value)
{
    // doesn't do anything for now
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