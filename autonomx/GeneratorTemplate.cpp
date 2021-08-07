#include "GeneratorTemplate.h"

GeneratorTemplate::GeneratorTemplate(int id, GeneratorMeta *meta) : Generator(id, meta)
{
    resetParameters();
    initialize();
}

GeneratorTemplate::~GeneratorTemplate()
{

}

void GeneratorTemplate::initialize()
{
    // re-initialize the cell matrix to all 0 values and w/ correct flat-2d size
    // this method is automatically called when either
    // the lattice width or height is changed (managed in the generic class)
    cells.resize(latticeWidth * latticeHeight);
}

void GeneratorTemplate::computeIteration(double deltaTime)
{
    // delta time scaled to speed factor
    totalTime += deltaTime * (speed / 100);

    // simple timer
    if (totalTime >= 1.0 / 60.0) {
        totalTime -= (1.0 / 60.0);

        // manipulate cell data here
        // what happens here counts as an iteration
    }
}

double GeneratorTemplate::getLatticeValue(int x, int y)
{
    int index = x % latticeWidth + y * latticeWidth;
    return cells[index];
}

void GeneratorTemplate::writeLatticeValue(int x, int y, double value)
{
    int index = x % latticeWidth + y * latticeWidth;
    cells[index] = value;
}

double GeneratorTemplate::getNoise() const
{
    return noise;
}

void GeneratorTemplate::writeNoise(const double noise)
{
    // prevent updating w/ same value
    if (this->noise == noise) return;

    // update value
    this->noise = noise;

    // this signal call is necessary for the changes to be reflected in the GUI
    // hooked to the corresponding GeneratorFacade
    valueChanged("noise", QVariant(noise));

    // regular signaling
    noiseChanged(noise);
}
