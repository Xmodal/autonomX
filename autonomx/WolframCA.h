//#ifndef WOLFRAMCA_H
//#define WOLFRAMCA_H

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
    bool        flagDebug               = true;

    // cells of lattice
    std::vector<double> cells;

    // cell values
    double** cellValues;

    // properties and rules
    int rule;

    // timeScale variable
    double      timeScale = 30.0 / 1000.0;

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

signals:
    // QML signals
    void ruleChanged(int rule);
    void timeScaleChanged(double timeScale);
};

//#endif // WOLFRAMCA_H
