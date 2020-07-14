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

class SpikingNet : public Generator {
    // TODO: figure out how we decide to add / remove inputs. this should probably be a property that belongs to the Generator abstract class, rather than this.
    Q_OBJECT
    Q_PROPERTY(int neuronSize READ getNeuronSize WRITE writeNeuronSize NOTIFY neuronSizeChanged)
    Q_PROPERTY(double timeScale READ getTimeScale WRITE writeTimeScale NOTIFY timeScaleChanged)
    Q_PROPERTY(double inhibitoryPortion READ getInhibitoryPortion WRITE writeInhibitoryPortion NOTIFY inhibitoryPortionChanged)
    Q_PROPERTY(double inputPortion READ getInputPortion WRITE writeInputPortion NOTIFY inputPortionChanged)
    Q_PROPERTY(double outputPortion READ getOutputPortion WRITE writeOutputPortion NOTIFY outputPortionChanged)
    Q_PROPERTY(NeuronType::Enum inhibitoryNeuronType READ getInhibitoryNeuronType WRITE writeInhibitoryNeuronType NOTIFY inhibitoryNeuronTypeChanged)
    Q_PROPERTY(NeuronType::Enum excitatoryNeuronType READ getExcitatoryNeuronType WRITE writeExcitatoryNeuronType NOTIFY excitatoryNeuronTypeChanged)
    Q_PROPERTY(double inhibitoryNoise READ getInhibitoryNoise WRITE writeInhibitoryNoise NOTIFY inhibitoryNoiseChanged)
    Q_PROPERTY(double excitatoryNoise READ getExcitatoryNoise WRITE writeExcitatoryNoise NOTIFY excitatoryNoiseChanged)

    Q_PROPERTY(double STPStrength READ getSTPStrength WRITE writeSTPStrength NOTIFY STPStrengthChanged)
    Q_PROPERTY(double STDPStrength READ getSTDPStrength WRITE writeSTDPStrength NOTIFY STDPStrengthChanged)
    Q_PROPERTY(double decayConstant READ getDecayConstant WRITE writeDecayConstant NOTIFY decayConstantChanged)

    Q_PROPERTY(bool flagSTP READ getFlagSTP WRITE writeFlagSTP NOTIFY flagSTPChanged)
    Q_PROPERTY(bool flagSTDP READ getFlagSTDP WRITE writeFlagSTDP NOTIFY flagSTDPChanged)
    Q_PROPERTY(bool flagDecay READ getFlagDecay WRITE writeFlagDecay NOTIFY flagDecayChanged)

// enum
public:
    enum class NetworkType {
        RandomNetwork,
        SparseNetwork,
        UniformNetwork,
        GridNetwork
    };
    Q_ENUM(NetworkType)
    //Q_ENUM(NeuronType)

private:
    NetworkType networkType = NetworkType::GridNetwork;
    int         neuronSize = 1000;
    int         connectionsPerNeuron = 20; // this is used in any non-grid network
    int         randomSeed = 0;
    int         gridNetworkWidth = 20;
    double      gridNetworkConnectionRate = 0.01; // this is used in the grid network

    double      inhibitoryPortion = 0.2;
    int         inhibitorySize = neuronSize * inhibitoryPortion;
    NeuronType::Enum  inhibitoryNeuronType = NeuronType::ChatteringNeuron;
    double      inhibitoryInitWeight = -5.0;
    double      inhibitoryNoise = 3.0;

    NeuronType::Enum  excitatoryNeuronType = NeuronType::ChatteringNeuron;
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

    double STDPStrength = 1.0;
    double STPStrength = 1.0;

    bool        flagSTP                 = false;
    bool        flagSTDP                = true;
    bool        flagDecay               = false;
    bool        flagDirectConnection    = true;
    bool        flagRandomDevice        = true;
    bool        flagDebug               = false;

    // the neurons
    std::vector<Izhikevich> neurons;
    // the weights
    double** weights;
    std::vector<double> outputGroupSpiking;
    // the per-output group activation average
    std::vector<double> outputGroupActivation;

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
    inline void applyFiring();
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

    void initialize();
    void reset();
    void update(double deltaTime);
    void stimulation();
    void stimulation(double strength);
    void stimulation(int inputGroupIndex, double strength);
    void wholeStimulation();
    void wholeStimulation(double strength);
    void wholeNetworkStimulation();
    void wholeNetworkStimulation(double strength);

    double getOutputGroupSpiking(int outputGroupIndex);
    double getOutputGroupActivation(int outputGroupIndex);

public:
    SpikingNet();
    ~SpikingNet();

    void computeOutput(double deltaTime);

    int getNeuronSize() const;
    double getTimeScale() const;
    double getInhibitoryPortion() const;
    double getInputPortion() const;
    double getOutputPortion() const;
    NeuronType::Enum getInhibitoryNeuronType() const;
    NeuronType::Enum getExcitatoryNeuronType() const;
    double getInhibitoryNoise() const;
    double getExcitatoryNoise() const;
    double getSTPStrength() const;
    double getSTDPStrength() const;
    double getDecayConstant() const;
    bool getFlagSTP() const;
    bool getFlagSTDP() const;
    bool getFlagDecay() const;

public slots:
    void writeNeuronSize(int neuronSize);
    void writeTimeScale(double timeScale);
    void writeInhibitoryPortion(double inhibitoryPortion);
    void writeInputPortion(double inputPortion);
    void writeOutputPortion(double outputPortion);
    void writeInhibitoryNeuronType(NeuronType::Enum inhibitoryNeuronType);
    void writeExcitatoryNeuronType(NeuronType::Enum excitatoryNeuronType);
    void writeInhibitoryNoise(double inhibitoryNoise);
    void writeExcitatoryNoise(double excitatoryNoise);
    void writeSTPStrength(double STPStrength);
    void writeSTDPStrength(double STDPStrength);
    void writeDecayConstant(double decayConstant);
    void writeFlagSTP(bool flagSTP);
    void writeFlagSTDP(bool flagSTDP);
    void writeFlagDecay(bool flagDecay);

signals:
    void neuronSizeChanged(int neuronSize);
    void timeScaleChanged(double timeScale);
    void inhibitoryPortionChanged(double inhibitoryPortion);
    void inputPortionChanged(double inputPortion);
    void outputPortionChanged(double outputPortion);
    void inhibitoryNeuronTypeChanged(NeuronType::Enum inhibitoryNeuronType);
    void excitatoryNeuronTypeChanged(NeuronType::Enum excitatoryNeuronType);
    void inhibitoryNoiseChanged(double inhibitoryNoise);
    void excitatoryNoiseChanged(double excitatoryNoise);
    void STPStrengthChanged(double STPStrength);
    void STDPStrengthChanged(double STDPStrength);
    void decayConstantChanged(double decayConstant);
    void flagSTPChanged(bool flagSTP);
    void flagSTDPChanged(bool flagSTDP);
    void flagDecayChanged(bool flagDecay);
};
