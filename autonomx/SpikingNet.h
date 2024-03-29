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

#include <random>
#include <vector>

#include "Generator.h"
#include "Izhikevich.h"
#include "GeneratorField.h"

class SpikingNet : public Generator {
    // TODO: figure out how we decide to add / remove inputs. this should probably be a property that belongs to the Generator abstract class, rather than this.
    Q_OBJECT
    Q_PROPERTY(double inhibitoryPortion READ getInhibitoryPortion WRITE writeInhibitoryPortion NOTIFY inhibitoryPortionChanged)
    Q_PROPERTY(NeuronType inhibitoryNeuronType READ getInhibitoryNeuronType WRITE writeInhibitoryNeuronType NOTIFY inhibitoryNeuronTypeChanged)
    Q_PROPERTY(NeuronType excitatoryNeuronType READ getExcitatoryNeuronType WRITE writeExcitatoryNeuronType NOTIFY excitatoryNeuronTypeChanged)
    Q_PROPERTY(double inhibitoryNoise READ getInhibitoryNoise WRITE writeInhibitoryNoise NOTIFY inhibitoryNoiseChanged)
    Q_PROPERTY(double excitatoryNoise READ getExcitatoryNoise WRITE writeExcitatoryNoise NOTIFY excitatoryNoiseChanged)

    Q_PROPERTY(double STPStrength READ getSTPStrength WRITE writeSTPStrength NOTIFY STPStrengthChanged)
    Q_PROPERTY(double STDPStrength READ getSTDPStrength WRITE writeSTDPStrength NOTIFY STDPStrengthChanged)
    Q_PROPERTY(double decayHalfLife READ getDecayHalfLife WRITE writeDecayHalfLife NOTIFY decayHalfLifeChanged)

    Q_PROPERTY(bool flag_STPStrength READ getFlagSTP WRITE writeFlagSTP NOTIFY flagSTPChanged)
    Q_PROPERTY(bool flag_STDPStrength READ getFlagSTDP WRITE writeFlagSTDP NOTIFY flagSTDPChanged)
    Q_PROPERTY(bool flag_decayHalfLife READ getFlagDecay WRITE writeFlagDecay NOTIFY flagDecayChanged)

// enum
public:
    enum class NetworkType {
        RandomNetwork,
        SparseNetwork,
        UniformNetwork,
        GridNetwork
    };
    Q_ENUM(NetworkType)

private:
    NetworkType networkType = NetworkType::GridNetwork;
    int         connectionsPerNeuron = 20; // this is used in any non-grid network
    int         randomSeed = 0;
    double      gridNetworkConnectionRate = 0.01; // this is used in the grid network

    double      inhibitoryPortion = 0.2;
    int         inhibitorySize = latticeWidth * latticeHeight * inhibitoryPortion;
    NeuronType  inhibitoryNeuronType = NeuronType::ChatteringNeuron;
    double      inhibitoryInitWeight = -5.0;
    double      inhibitoryNoise = 3.0;

    NeuronType  excitatoryNeuronType = NeuronType::ChatteringNeuron;
    double      excitatoryInitWeight = 15.0;
    double      excitatoryNoise = 5.0;

    double      weightMax = 20.0;
    double      weightMin = 0.0;

    double      decayHalfLife = 10.0;
    double      decayConstant = std::pow(2.0, - 1.0 / decayHalfLife);

    double STDPStrength = 1.0;
    double STPStrength = 1.0;

    bool        flagSTP                 = false;
    bool        flagSTDP                = false;
    bool        flagDecay               = false;
    bool        flagDirectConnection    = true;
    bool        flagRandomDevice        = true;
    bool        flagDebug               = false;

    // the neurons
    std::vector<Izhikevich> neurons;
    // the weights
    double** weights;

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

    inline void applyIzhikevich(double deltaTime);
    inline void applyConnections();
    inline void applyFiring();
    inline void applyDecay(double deltaTime);
    inline void computeSTDP(double deltaTime);
    inline void computeSTP(double deltaTime);
    inline double computeSTPForNeuron(int index, bool isFiring, double deltaTime);

    void writeNetworkType(QString networkType);
    void setRandomNetwork();
    void setSparseGraph();
    void setSparseNetwork();
    void setUniformNetwork();
    void setChainNetwork();
    void setGridNetwork();

public:
    SpikingNet(int id, GeneratorMeta * meta);
    ~SpikingNet();

    void computeIteration(double deltaTime) override;
    void initialize() override;
    double getLatticeValue(int x, int y) override;
    void writeLatticeValue(int x, int y, double value) override;

    int getNeuronSize() const;
    double getInhibitoryPortion() const;
    NeuronType getInhibitoryNeuronType() const;
    NeuronType getExcitatoryNeuronType() const;
    double getInhibitoryNoise() const;
    double getExcitatoryNoise() const;
    double getSTPStrength() const;
    double getSTDPStrength() const;
    double getDecayHalfLife() const;
    bool getFlagSTP() const;
    bool getFlagSTDP() const;
    bool getFlagDecay() const;

    void writeNeuronSize(int neuronSize);
    void writeInhibitoryPortion(double inhibitoryPortion);
    void writeNeuronType(QString neuronMetaType, int neuronType);
    void writeInhibitoryNeuronType(NeuronType inhibitoryNeuronType);
    void writeExcitatoryNeuronType(NeuronType excitatoryNeuronType);
    void writeInhibitoryNoise(double inhibitoryNoise);
    void writeExcitatoryNoise(double excitatoryNoise);
    void writeSTPStrength(double STPStrength);
    void writeSTDPStrength(double STDPStrength);
    void writeDecayHalfLife(double decayHalfLife);
    void writeFlagSTP(bool flagSTP);
    void writeFlagSTDP(bool flagSTDP);
    void writeFlagDecay(bool flagDecay);

signals:
    void neuronSizeChanged(int neuronSize);
    void inhibitoryPortionChanged(double inhibitoryPortion);
    void inhibitoryNeuronTypeChanged(NeuronType inhibitoryNeuronType);
    void excitatoryNeuronTypeChanged(NeuronType excitatoryNeuronType);
    void inhibitoryNoiseChanged(double inhibitoryNoise);
    void excitatoryNoiseChanged(double excitatoryNoise);
    void STPStrengthChanged(double STPStrength);
    void STDPStrengthChanged(double STDPStrength);
    void decayHalfLifeChanged(double decayHalfLife);
    void flagSTPChanged(bool flagSTP);
    void flagSTDPChanged(bool flagSTDP);
    void flagDecayChanged(bool flagDecay);
};
