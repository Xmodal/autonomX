#include "WolframCA.h"

WolframCA::WolframCA(int id) : Generator(id, "WolframCA", "WCA", "Wolfram CA description")
{

}

WolframCA::~WolframCA()
{

}

void WolframCA::computeIteration(double deltaTime)
{
    // execute CA here
}

void WolframCA::initialize()
{
    // re-init algorithm here
}

void WolframCA::resetParameters()
{
    // reset params here
}

double WolframCA::getLatticeValue(int x, int y)
{
    return 0.0;
}

void WolframCA::writeLatticeValue(int x, int y, double value)
{
    // write values to lattice
}

int WolframCA::getRule()
{
    return rule;
}

void WolframCA::setRule()
{
    this->rule = rule;
    // make sure you follow this signal structure when you write a property!
    emit ruleChanged(rule);
    emit valueChanged("rule", rule);
}
