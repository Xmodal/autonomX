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

#include "Generator.h"
#include "Izhikevich.h"

#include <random>
#include <vector>

enum NetworkType {
    randomNetwork,
    sparseNetwork,
    uniformNetwork,
    gridNetwork
};

class SpikingNet : public Generator {
private:
    NetworkType networkType = gridNetwork;
    int         neuronSize = 1000;
    int         connectionsPerNeuron = 20;
    int         randomSeed = 0;
    int         gridNetworkWidth = 20;
    double      gridNetworkConnectionRate = 0.01;

    double      inhibitoryPortion = 0.2;
    int         inhibitorySize = neuronSize * inhibitoryPortion;
    NeuronType  inhibitoryNeuronType = chatteringNeuron;
    double      inhibitoryInitWeight = -5.0;
    double      inhibitoryNoise = 3.0;

    NeuronType  excitatoryNeuronType = chatteringNeuron;
    double      excitatoryInitWeight = 15.0;
    double      excitatoryNoise = 5.0;

    double      inputPortion = 0.2;
    int         inputSize = (neuronSize - inhibitorySize) * inputPortion;
    int         inputGroupSize = 1;

    double      outputPortion = 0.2;
    int         outputSize = (neuronSize - inhibitorySize) * outputPortion;
    int         outputGroupSize = 1;

    double      weightMax = 20.0;
    double      weightMin = 0.0;

    double      stimStrength = 6.0;
    double      decayConstant = 0.9995;

    double      timeScale = 30.0 / 1000.0;

    bool        flagSTP                 = false;
    bool        flagSTDP                = true;
    bool        flagDecay               = true;
    bool        flagDirectConnection    = true;
    bool        flagRandomDevice        = true;
    bool        flagDebug               = true;

    // the neurons
    std::vector<Izhikevich> neurons;
    // the weights
    double** weights;
    // the per-output group spiking average
    std::vector<double> outputSpiking;

    // STDP
    std::vector<int> STDPTimes;
    double STDPWindow = 20.0 / 1000.0;

    // STP
    double* STPu;
    double* STPx;
    double* STPw;

    std::mt19937 randomGenerator;

    inline int indexInhibitoryNeuron(int i);
    inline int indexExcitatoryNeuron(int i);
    inline int indexInputNeuron(int i);
    inline int indexOutputNeuron(int i);
    
    inline void updateNeurons(double deltaTime);
    inline void updateInput();
    inline void updateInputDebug();
    inline void checkFiring();
    inline void computeSTDP(double deltaTime);
    inline void computeSTP(double deltaTime);
    inline double getSTPValue(int index, bool isFiring, double deltaTime);
    inline void decay(double deltaTime);

    void setRandomNetwork();
    void setSparseGraph();
    void setSparseNetwork();
    void setUniformNetwork();
    void setChainNetwork();
    void setGridNetwork();

    void init();
    void update(double deltaTime);
    void stimulation();
    void stimulation(double strength);
    void stimulation(int inputGroupIndex, double strength);
    void wholeStimulation();
    void wholeStimulation(double strength);
    void wholeNetworkStimulation();
    void wholeNetworkStimulation(double strength);
    int  getSpikedOutput(int outputGroupIndex);
    
public:
    SpikingNet();
    ~SpikingNet();

    void computeOutput(double deltaTime);
};
