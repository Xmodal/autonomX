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

#pragma once

#include <iostream>
#include <vector>

#include <NeuronType.h>

class Izhikevich {
private:
    double potentialThreshold;
    double u;
    double v;
    NeuronType::Enum type;
    double I;
    double a, b, c, d;

public:
    Izhikevich();
    ~Izhikevich();

    void update(double deltaTime);
    void setParam(NeuronType::Enum type, double a, double b, double c, double d, double u, double v, double I);
    void setNeuronType(NeuronType::Enum type);
    bool applyFiring(); // checks if the neuron is firing and updates the differential equationa accordingly
    NeuronType::Enum getNeuronType();
    bool isFiring();
    
    void setA(double a){ this->a = a; };
    void setB(double b){ this->b = b; }
    void setC(double c){ this->c = c; }
    void setD(double d){ this->d = d; }

    double getA();
    double getB();
    double getC();
    double getD();
    double getU();
    double getV();
    double getI();
    double getPotentialThreshold();
    void setI(double I);
    void addToI(double deltaI);
    
    bool firing;

};
