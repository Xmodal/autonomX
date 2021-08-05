#pragma once

#include <vector>

#include "Generator.h"

class GeneratorTemplate : public Generator
{
    Q_OBJECT
public:
    GeneratorTemplate(int id, GeneratorMeta * meta);
    //~GeneratorTemplate();

    void initialize() override;
    void computeIteration(double deltaTime) override;
    double getLatticeValue(int x, int y) override;
    void writeLatticeValue(int x, int y, double value) override;

private:
    std::vector<double> cells;
    double totalTime = 0;
};
