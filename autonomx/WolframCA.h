//#ifndef WOLFRAMCA_H
//#define WOLFRAMCA_H

#pragma once
#include <vector>
#include<random>
#include "Generator.h"


class WolframCA : public Generator
{
    Q_OBJECT
    Q_PROPERTY(int rule READ getRule WRITE writeRule NOTIFY ruleChanged)
    Q_PROPERTY(double timeScale READ getTimeScale WRITE writeTimeScale NOTIFY timeScaleChanged)
    Q_PROPERTY(double randSeed READ getRandSeed WRITE writeRandSeed NOTIFY randSeedChanged)

private:

    // Debugging
    bool  flagDebug = true;

    // cells of lattice
    std::vector<double> cells;

    // Binary conversion of decimal user input in future starting by rule 90
    int ruleset[8];// = {0,1,0,1,1,0,1,0};// {1,1,0,1,1,1,1,0};

    // cell values
    double* cellValues;

    // properties and rules; default rule set to 102
    int rule = 102;

    //random seed property
    double randSeed = 0.0;

    //random generator to initialize cells
    std::mt19937 randomGenerator;

    // timeScale variable
    double timeScale = 30.0 / 1000.0;

    // global iteration counter
    int iterationNumber;

    //to store the previous rule
    int prev_rule;

    // WolframCA specific variables
    int currentGeneration = 0;
    int lastGeneration;


public:

    WolframCA(int id);
    ~WolframCA();

    // overrides / implementing necessary virtual functions from Generator class
    void computeIteration(double deltaTime) override;
    void initialize() override;
    void resetParameters() override;
    double getLatticeValue(int x, int y) override;
    void writeLatticeValue(int x, int y, double value) override;
    double sigmoid(double value);
    void generate(int r);
    int findCellValue(int left,int middle, int right);

    // accessors / mutators
    int getRule();
    void writeRule(int rule);

    void writeTimeScale(double timeScale);
    double getTimeScale() const;

    //random seed for random initialization
    void writeRandSeed(double randSeed);
    double getRandSeed() ;

signals:
    // QML signals
    void ruleChanged(int rule);
    void timeScaleChanged(double timeScale);
    void randSeedChanged(double randSeed);
};

//#endif // WOLFRAMCA_H
