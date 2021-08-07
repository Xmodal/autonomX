#pragma once

#include <vector>

#include "Generator.h"

class GeneratorTemplate : public Generator
{
    Q_OBJECT
    Q_PROPERTY(double noise READ getNoise WRITE writeNoise NOTIFY noiseChanged)
public:
    // constructor/destructor
    GeneratorTemplate(int id, GeneratorMeta * meta);
    ~GeneratorTemplate();

    // overrides
    void initialize() override;
    void computeIteration(double deltaTime) override;
    double getLatticeValue(int x, int y) override;
    void writeLatticeValue(int x, int y, double value) override;

    // getters
    double getNoise() const;

    // setters
    void writeNoise(double const noise);

private:
    std::vector<double> cells;
    double totalTime = 0;

    // property members
    double noise = 0.0;

signals:
    // Qt signals
    void noiseChanged(double noise);
};
