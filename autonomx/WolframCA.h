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

#include "Generator.h"


class WolframCA : public Generator
{
    Q_OBJECT
    Q_PROPERTY(int rule READ getRule WRITE writeRule NOTIFY ruleChanged)
    Q_PROPERTY(double timeScale READ getTimeScale WRITE writeTimeScale NOTIFY timeScaleChanged)

private:

    // Debugging
    bool flagDebug = false;

    // cells of lattice
    std::vector<double> cells;

    // cell values
    double* cellValues;

    // properties and rules
    int rule;

    // timeScale variable
    double timeScale = 30.0 / 1000.0;

    // global iteration counter
    int iterationNumber;        // iteration = 1 tick in global clock

    // WolframCA specific variables
    int currentGeneration = 0;  // generation = each 1D row of cells = 1000 iterations
    int lastGeneration;         // last generation = ensures loop to beginning of lattice on next generation


public:

    WolframCA(int id);
    ~WolframCA();

    // overrides / implementing necessary virtual functions from Generator class
    void computeIteration(double deltaTime) override;
    void initialize() override;
    void resetParameters() override;
    double getLatticeValue(int x, int y) override;
    void writeLatticeValue(int x, int y, double value) override;

    // accessors / mutators
    int getRule();
    void writeRule(int rule);

    void writeTimeScale(double timeScale);
    double getTimeScale() const;

//    double getCellValue();
//    void setCellValue(double);

    int modValue = 1;

signals:
    // QML signals
    void ruleChanged(int rule);
    void timeScaleChanged(double timeScale);
};
