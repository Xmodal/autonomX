#pragma once
#include <vector>

#include "Generator.h"

class GameOfLife : public Generator
{
    Q_OBJECT
    Q_PROPERTY(int rule READ getRule WRITE writeRule NOTIFY ruleChanged)
private:
    // computation variables here etc etc
    std::vector<double> cells;

    // cell values
    bool* cellLife;

    // global iteration counter
    int iterationNumber;

    // GOL specific variables
    int currentGeneration;
    int lastGeneration;
    bool gameOver = false;


public:
    GameOfLife(int id);
    ~GameOfLife();

    // overrides
    void computeIteration(double deltaTime) override;
    void initialize() override;
    void resetParameters() override;
    double getLatticeValue(int x, int y) override;
    void writeLatticeValue(int x, int y, double value) override;

    // prop hooks
    int getRule();

signals:
    // QML signals
    void ruleChanged(int rule);
};
