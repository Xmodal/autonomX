// Copyright 2020, Atsushi Masumori & Xmodal
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

#include "Izhikevich.h"

#include <random>

Izhikevich::Izhikevich() {
    
    firing = false;
    
    //set param as default Ne.
    setNeuronType(spikingNeuronRandomized);
    u = b*v;
    I = 0.;
    potentialThreshold = 20.;
}

Izhikevich::~Izhikevich() {
}

void Izhikevich::setParam(NeuronType _type, double _a, double _b, double _c, double _d, double _u, double _v, double _I) {
    this->type = _type;
    this->a = _a;
    this->b = _b;
    this->c = _c;
    this->d = _d;
    this->v = _u;
    this->d = _v;
    this->I = _I;
}

void Izhikevich::setNeuronType(NeuronType _type) {
    type = _type;
    switch(type) {
        case spikingNeuron: {
            a = 0.02;
            b = 0.2;
            c = -65.;
            d = 8.;
            break;
        }
        case spikingNeuronRandomized: {
            std::mt19937 randomGenerator;
            std::uniform_real_distribution<> randomUniform(0.0, 1.0);
            double random = randomUniform(randomGenerator);
            a = 0.02;
            b = 0.2;
            c = -65. + 15. * random * random;
            d = 8. - 6.* random * random;
            break;
        }
        case resonatorNeuron: {
            a = 0.1;
            b = 0.2;
            c = -65.;
            d = 2.;
            break;
        }
        case resonatorNeuronRandomized: {
            std::mt19937 randomGenerator;
            std::uniform_real_distribution<> randomUniform(0.0, 1.0);
            double random = randomUniform(randomGenerator);
            a = 0.02 + 0.08 * random;
            b = 0.25 - 0.05 * random;
            c = -65.;
            d = 2.;
            break;
        }
        case chatteringNeuron: {
            a = 0.02;
            b = 0.2;
            c = -50.;
            d = 2.;
            break;
        }
        case inhibitoryNeuron: {
            a = 0.1;
            b = 0.2;
            c = -65.;
            d = 2.;
            break;
        }
        case inhibitoryNeuronRandomized: {
            std::mt19937 randomGenerator;
            std::uniform_real_distribution<> randomUniform(0.0, 1.0);
            double random = randomUniform(randomGenerator);
            a = 0.1;
            b = 0.25 - 0.05 * random;
            c = -65.;
            d = 2.;
            break;
        }
        case excitatoryNeuron : {
            a = 0.02;
            b = 0.2;
            c = -50.;
            d = 2.;
            break;
        }
        case excitatoryNeuronRandomized : {
            std::mt19937 randomGenerator;
            std::uniform_real_distribution<> randomUniform(0.0, 1.0);
            double random = randomUniform(randomGenerator);
            a = 0.02;
            b = 0.2;
            c = -65. + 15. * random * random;
            d = 8. - 6. * random * random;
            break;
        }
    }
    v = -65.;
    u = d;
    I = 0.;
}


void Izhikevich::update(double deltaTime) {
    double deltaTimeMillis = deltaTime * 1000.0;
    v += deltaTimeMillis * 0.5 * (0.04 * v * v + 5 * v + 140 - u + I);
    v += deltaTimeMillis * 0.5 * (0.04 * v * v + 5 * v + 140 - u + I);
    u += deltaTimeMillis * a * (b * v - u);
}

bool Izhikevich::applyFiring() {
    
    if(v > potentialThreshold) {
        firing = true;
        v = c;
        u = u + d;
    } else {
        firing = false;
    }
    return(firing);
}

NeuronType Izhikevich::getNeuronType() {
    return(type);
}

bool Izhikevich::isFiring() {
    return(this->firing);
}

double Izhikevich::getA() {
    return(this->a);
}

double Izhikevich::getB() {
    return(this->b);
}

double Izhikevich::getC() {
    return(this->c);
}

double Izhikevich::getD() {
    return(this->d);
}

double Izhikevich::getU() {
    return(u);
}

double Izhikevich::getV() {
    return(v);
}

double Izhikevich::getI() {
    return(this->I);
}

void Izhikevich::setI(double i) {
    this->I = i;
}

void Izhikevich::addToI(double a) {
    this->I = this->I + a;
}
