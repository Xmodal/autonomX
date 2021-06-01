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

    // properties and rules; default rule set to 90
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
    int currentGeneration = 1;
    int lastGeneration;


public:

    WolframCA(int id, GeneratorMeta * meta);
    ~WolframCA();

    // overrides / implementing necessary virtual functions from Generator class
    void computeIteration(double deltaTime) override;
    void initialize() override;
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

    void writeRandSeed(double randSeed);
    double getRandSeed() ;

signals:
    // QML signals
    void ruleChanged(int rule);
    void timeScaleChanged(double timeScale);
    void randSeedChanged(double randSeed);
};