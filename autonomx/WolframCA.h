//#ifndef WOLFRAMCA_H
//#define WOLFRAMCA_H

#pragma once

#include "Generator.h"


class WolframCA : public Generator
{
    Q_OBJECT
    Q_PROPERTY(int rule READ getRule WRITE writeRule NOTIFY ruleChanged)

private:

    std::vector<double> cells;

    // properties and rules
    int rule;

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
    void setRule();

signals:
    // QML signals
    void ruleChanged(int rule);
};

//#endif // WOLFRAMCA_H