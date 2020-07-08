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

#include "SpikingNet.h"

#include <chrono>
#include <QThread>
#include <QDebug>

// ############################### initialization routines ###############################

SpikingNet::SpikingNet() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (SpikingNet):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // default values for descriptive properties
    name = "Spiking Neural Network";
    type = "SNN";
    description = "An interconnected network of biologically-modeled neurons.";
    outputMonitor = 0;

    initialize();
}

SpikingNet::~SpikingNet() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (SpikingNet):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    reset();
}


void SpikingNet::initialize() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "initialize:\t\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // set random seed
    if(flagRandomDevice) {
        std::random_device random;
        randomGenerator.seed(random());
    } else {
        randomGenerator.seed(randomSeed);
    }

    // setup vectors
    neurons.resize(neuronSize);

    outputGroupSpiking.resize(outputGroupSize, 0);
    outputGroupActivation.resize(outputGroupSize, 0);

    STDPTimes.resize(neuronSize, 0);

    input.resize(inputGroupSize, 0);
    output.resize(outputGroupSize, 0);

    // allocate memory for STP variables
    STPu = new double[neuronSize];
    STPx = new double[neuronSize];
    STPw = new double[neuronSize];

    // initialize STP variables
    for(int i = 0; i < neuronSize; ++i) {
        STPw[i] = 1.0;
        STPx[i] = 1.0;
        STPu[i] = 0.0;
    }

    // allocate memory for weights
    weights = new double*[neuronSize];
    for(int i = 0; i < neuronSize; ++i) {
        weights[i] = new double[neuronSize];
    }

    // initialize weights
    for(int i = 0; i < neuronSize; ++i) {
        for(int j = 0; j < neuronSize; ++j) {
            weights[i][j] = 0.;
        }
    }

    // set neuron types
    for(int i = 0; i < neuronSize; ++i) {
        if(i < inhibitorySize) {
            neurons[i].setNeuronType(inhibitoryNeuronType);
        } else{
            neurons[i].setNeuronType(excitatoryNeuronType);
        }
    }

    // set network type
    switch(networkType) {
        case NetworkType::SparseNetwork:
            setSparseNetwork();
            break;
        case NetworkType::RandomNetwork:
            setRandomNetwork();
            break;
        case NetworkType::UniformNetwork:
            setUniformNetwork();
            break;
        case NetworkType::GridNetwork:
            setGridNetwork();
            break;
    }
}

void SpikingNet::reset() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "reset:\t\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // delete STP variables
    delete[] STPu;
    delete[] STPx;
    delete[] STPw;
    STPu = 0;
    STPx = 0;
    STPw = 0;

    // delete weights
    for(int i = 0; i < neuronSize; ++i) {
        delete[] weights[i];
        weights[i] = 0;
    }
    delete[] weights;
    weights = 0;
}

// inhibitory neurons are first, the rest is excitatory neurons
// input and output neurons are always in the excitatory neurons

// ------ inhibitory ------ | --------------------------- excitatory ------------------------------ |
// ------ unassigned ------ | ------ input ------ | ------ output ------ | ------ unassigned ------ |

int SpikingNet::indexInhibitoryNeuron(int i) {
    // expects an index [0, Inhibitory_Size - 1]
    return i;
}

int SpikingNet::indexExcitatoryNeuron(int i) {
    // expects an index [0, Neuron_Size - Inhibitory_Size - 1]
    return i + inhibitorySize;
}

int SpikingNet::indexInputNeuron(int i) {
    // expects an index [0, Input_Size - 1]
    return i + inhibitorySize;
}

int SpikingNet::indexOutputNeuron(int i) {
    // expects an index [0, Output_Size - 1]
    return i + inhibitorySize + inputSize;
}

void SpikingNet::setSparseNetwork() {

    std::uniform_int_distribution<> randomNeurons(0, neuronSize - 1);
    std::uniform_real_distribution<> randomUniform(0.0, 1.0);

    int connectionSum = 0;
    int destinationArray[connectionsPerNeuron];

    // in case of non fully connected
    if(connectionsPerNeuron != neuronSize) {
        for(int source = 0; source < neuronSize; ++source) {
            for(int n = 0; n < connectionsPerNeuron; n++) {
                // initialize to something that won't be matched when we check if the candidate is already used
                destinationArray[n] = -1;
            }

            int count = 0;
            while(count < connectionsPerNeuron) {
                int candidate = randomNeurons(randomGenerator);
                // check wether the candidate_id is already included in dest_array.
                bool isIncluded = false;
                for(int i = 0; i < connectionsPerNeuron; i++) {
                    if(destinationArray[i] == candidate) {
                        isIncluded = true;
                        break;
                    }
                }

                if(!isIncluded) {
                    destinationArray[count] = candidate;
                    count++;
                    connectionSum++;
                }
            }

            for(int j = 0; j < connectionsPerNeuron; j++) {
                int destination = destinationArray[j];
                if(destination != source) {
                    if(destination >= 0 && destination < neuronSize) {
                        if(source < inhibitorySize) {
                            weights[source][destination] = inhibitoryInitWeight * randomUniform(randomGenerator);
                        } else {
                            weights[source][destination] = excitatoryInitWeight * randomUniform(randomGenerator);
                        }
                    }
                }
            }
        }
    }

    // in case of fully connected
    else {
        for(int i = 0; i < neuronSize; ++i) {
            for(int j = 0; j < neuronSize; j++) {
                if(i != j) {
                    if(i < inhibitorySize) {
                        weights[i][j] = inhibitoryInitWeight * randomUniform(randomGenerator);
                    } else {
                        weights[i][j] = excitatoryInitWeight * randomUniform(randomGenerator);
                    }
                    connectionSum++;
                }
            }
        }
    }

    // TODO: this kind of sucks, make it so that this doesn't happen in the first place?
    // delete direct connection between input and output
    if(flagDirectConnection == false) {
        for(int source = 0; source < inputSize; source++) {
            for(int destination = 0; destination < outputSize; destination++) {
                weights[indexInputNeuron(source)][indexOutputNeuron(destination)] = 0.0;
            }
        }
    }
}

// fully connected with same weight : This is for debug.
void SpikingNet::setUniformNetwork() {

    int connectionSum = 0;

    for(int i = 0; i < neuronSize; ++i) {
        for(int j = 0; j < neuronSize; j++) {

            if(i != j) {
                if(i < inhibitorySize) {
                    weights[i][j] = inhibitoryInitWeight;
                } else {
                    weights[i][j] = excitatoryInitWeight;
                    if(j % 3 == 0) weights[i][j] = excitatoryInitWeight * 1.50;
                }
                connectionSum++;
            }
        }
    }
}

// fully connected network with random weight
void SpikingNet::setRandomNetwork() {

    std::uniform_real_distribution<> randomUniform(0.0, 1.0);

    int connectionSum = 0;

    for(int i = 0; i < neuronSize; ++i) {
        for(int j = 0; j < neuronSize; j++) {

            if(i != j) {
                if(i < inhibitorySize) {
                    weights[i][j] = inhibitoryInitWeight * randomUniform(randomGenerator);

                } else {
                    weights[i][j] = excitatoryInitWeight * randomUniform(randomGenerator);
                }
                connectionSum++;
            }
        }
    }
}

// fully connected with same weight : This is for debug.
// TODO: grid network ignores the connectionsPerNeuron param and instead uses gridNetworkConnectionRate. rewrite setGridNetwork() to use connectionsPerNeuron instead
void SpikingNet::setGridNetwork() {

    std::uniform_real_distribution<> randomUniform(0.0, 1.0);

    for(int i = 0; i < neuronSize; ++i) {
        int row = i / gridNetworkWidth;
        int col = i % gridNetworkWidth;
        for(int j = 0; j < neuronSize; j++) {
            int row_target = j / gridNetworkWidth;
            int col_target = j % gridNetworkWidth;
            if(i < inhibitorySize) {
                if(i != j && randomUniform(randomGenerator) < gridNetworkConnectionRate) {
                    weights[i][j] = inhibitoryInitWeight * randomUniform(randomGenerator); // HERE!!!
                } else {
                    weights[i][j] = 0;
                }
            } else if (j < inhibitorySize) {
                if(i != j && randomUniform(randomGenerator) < gridNetworkConnectionRate) {
                    weights[i][j] = excitatoryInitWeight * randomUniform(randomGenerator); // HERE!!!
                } else {
                    weights[i][j] = 0;
                }
            } else if (i != j && (abs(row - row_target) <= 1 && (abs(col - col_target) <= 1  || abs(col - col_target) == 8))) {
                weights[i][j] = excitatoryInitWeight * randomUniform(randomGenerator);  // HERE!!!
            } else {
                weights[i][j] = 0;
            }
        }
    }
}

// ############################### update routines and output calculation ###############################

void SpikingNet::computeOutput(double deltaTime) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "computeOutput:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // apply input stimulation
    for(int i = 0; i < inputGroupSize; i++) {
        stimulation(i, input[i]);
    }

    // update, applying time scale
    update(deltaTime * timeScale);

    // write output
    for(int i = 0; i < outputGroupSize; i++) {
        output[i] = getOutputGroupActivation(i);
    }

}

void SpikingNet::update(double deltaTime) {

    if(flagDecay) decay(deltaTime);
    if(flagSTDP)  computeSTDP(deltaTime);
    if(flagSTP)   computeSTP(deltaTime);

    updateInput();
    updateNeurons(deltaTime);

    applyFiring();
}

double sigmoid(double value) {
    // https://www.desmos.com/calculator/ikdusaa9yh
    // sigmoid function centered at 0
    // df/dx = 1 at x = 0
    // f(0) = 0
    // f(infinity) = 1
    // f(-infinity) = -1

    return 2.0 / (1.0 + exp(- 2.0 * value)) - 1.0;
}

double softKnee(double value, double window) {
    // https://www.desmos.com/calculator/34ccgcquj3
    // soft clip function
    // is linear in the range [-window, window]
    // converges towards 1 at infinity
    // converges towards -1 at -infinity
    // second derivative is smooth at all points

    if(- window < value && value < window)
        return value;

    double scalingFactor = 1.0 / (1.0 - window);

    if(value > 0) {
        return sigmoid(scalingFactor * (value - window)) / scalingFactor + window;
    } else {
        return sigmoid(scalingFactor * (value + window)) / scalingFactor - window;
    }
}

double softKneePositive(double value, double window) {
    // indentical to function above except it uses the range [0, 1] instead
    // the window parameter also behaves diferently: it defines the total size of the linear region
    // the region [1/2 - window/2, 1/2 + window/2] is linear
    return (softKnee(2.0 * value - 1.0, window) + 1.0) * 0.5;
}

void SpikingNet::applyFiring() {
    // apply firing status to neurons
    for(int i = 0; i < neuronSize; i++) {
        neurons[i].applyFiring();
    }
    // reset group output variables
    for(int i = 0; i < outputGroupSize; i++) {
        outputGroupSpiking[i] = 0;
        outputGroupActivation[i] = 0;
    }
    int sizePerGroup = (outputSize / outputGroupSize);
    double averagingConstant = 1.0 / (double) sizePerGroup;
    int total = 0;
    for(int i = 0; i < outputSize; i++) {
        // get the index of the output neuron
        int index = indexOutputNeuron(i);
        // get the index of the group this neuron belongs to. min is there to prevent an edge case where the number of group doesn't perfectly divide the number of output neurons.
        int indexGroup = std::min<int>(i / sizePerGroup, outputGroupSize - 1);
        // check to see if the neuron is firing
        if(neurons[index].isFiring()) {
            total++;
            // apply increment with averaging. this isn't perfect if the last group size isn't exactly the same as the other ones, but probably shouldn't be a big problem.
            outputGroupSpiking[indexGroup] += 1.0 * averagingConstant;
        }
        // update output group activation data
        outputGroupActivation[indexGroup] += (neurons[index].getV() - neurons[index].getC()) / (neurons[index].getPotentialThreshold() - neurons[index].getC()) * averagingConstant;
    }
    // apply soft-clamp on output group activation
    for(int i = 0; i < outputGroupSize; i++) {
        outputGroupActivation[i] = softKneePositive(outputGroupActivation[i], 0.8);
        if(flagDebug) {
            qDebug() << "output group " << i << " activation: " << outputGroupActivation[i];
        }
    }
}

// returns the per-output group spiking average
double SpikingNet::getOutputGroupSpiking(int outputGroupIndex) {
    return outputGroupSpiking[outputGroupIndex];
}

// returns the per-output group activation average
double SpikingNet::getOutputGroupActivation(int outputGroupIndex) {
    return outputGroupActivation[outputGroupIndex];
}

void SpikingNet::updateInput() {

    // setup normal distribution random function
    std::normal_distribution<> randomUniform(0.0, 1.0);

    // pseudo thalamus noise-input
    for(int i = 0; i < neuronSize; ++i) {
        if(i < inhibitorySize) {
            neurons[i].addToI(inhibitoryNoise * randomUniform(randomGenerator));
        }else{
            neurons[i].addToI(excitatoryNoise * randomUniform(randomGenerator));
        }
    }

    // input from connected neurons with STP
    for(int i = 0; i < neuronSize; ++i) {
        if(neurons[i].isFiring()) {
            for(int j = 0; j < neuronSize; ++j) {
                if(i != j) {
                    if(i > inhibitorySize)
                        neurons[j].addToI((float) weights[i][j] * (float)STPw[i]);
                    else
                        neurons[j].addToI((float) weights[i][j]);
                }
            }
        }
    }

}

void SpikingNet::updateNeurons(double deltaTime) {

    // update differential equation
    for(int i = 0; i < neuronSize; ++i) {
        neurons[i].update(deltaTime);
        neurons[i].setI(0.0);
    }

}


void SpikingNet::computeSTDP(double deltaTime) {

    // read http://www.scholarpedia.org/article/Spike-timing_dependent_plasticity for more info on the maths behind this

    // essentially, this is a technique for dynamically changing the strength of connections according to the the firing patterns of connected neurons.
    // spike timing dependent plasticity works by strengthening connections that "predict" spiking patterns, and in a way "speed up" the propagation of spikes, in the sense that if a neuron A has a tendency to fire just before another neuron B, the connection from A to B will become stronger. inversely, the connection from B to A will become weaker.

    // the way the connection strength changes is decided by a function made up of two exponentials. it looks a bit like this.

    //                            . ^
    //                           .  |
    //                        ...   |
    //  connection       .....      |
    //  strength        ------------|----------->
    //  change                      |      .....
    //  A -> B                      |   ...
    //                              |  .
    //                              | .
    //
    //                       timing difference
    //
    //                A before B        B before A
    //
    //              A -> B becomes    A -> B becomes
    //                 stronger           weaker

    // each neuron has a STDPTimes[i] value that store how long ago it fired
    // the weight change is applied using the above function if the time difference between two neurons firing is smaller or equal to STDPWindow

    for(int i = inhibitorySize; i < neuronSize; ++i) {
        if(neurons[i].isFiring()) {
            // if the neuron is currently firing, set its STDPTimes to 0.
            STDPTimes[i] = 0;
        } else {
            // else, increase it by deltaTime
            STDPTimes[i] += deltaTime;
        }

    }

    double deltaTimeMillis = deltaTime * 1000.0;
    double d;
    for(int i = inhibitorySize; i < neuronSize; i++) {
        if(neurons[i].isFiring()) {
            for(int j = inhibitorySize; j < neuronSize; j++) {

                if(STDPTimes[j] <= STDPWindow && STDPTimes[j] != 0 && i != j) {
                    // another (uniquely different) neuron has fired in the last STDPTau frames (excluding the current frame)

                    // this is part of the exponential function described above
                    d = deltaTimeMillis * (0.1 * STDPStrength * pow(0.95, (1000.0 * STDPTimes[j])));

                    // update the weight from j to i, should be increased since j fired before i.
                    if(weights[j][i] != 0.0) {
                        weights[j][i] += d;
                        if (weights[j][i] > weightMax) weights[j][i] = weightMax;

                    }

                    // update the weight from i to j, should be lowered since i fired before j. careful! only the indexing changes.
                    if(weights[i][j] != 0.0) {
                        weights[i][j] -= d;
                        if(weights[i][j] < weightMin) weights[i][j] = weightMin;

                    }
                }
            }
        }
    }
}

void SpikingNet::computeSTP(double deltaTime) {
    for(int i = inhibitorySize; i < neuronSize; ++i) {
        STPw[i] = STPStrength * getSTPValue(i, neurons[i].isFiring(), deltaTime);
    }
}

double SpikingNet::getSTPValue(int index, bool isFiring, double deltaTime) {
    double deltaTimeMillis = deltaTime * 1000.0;

    // see http://www.rmki.kfki.hu/~banmi/elte/synaptic.pdf for paper describing the mechanics of this

    double U = 0.2; // mV. baseline level for u

    double s = isFiring ? 1.0 : 0.0;    // s indicates whether the neuron is currently firing or not (0 or 1)
    double u = STPu[index];             // u represents the amount of resources used by the creation of a spike.
                                        // normalized (0, U), baseline is U
    double x = STPx[index];             // x represents the amount of resources avaliable
                                        // normalized (0, 1), baseline is 1

    // when a spike is created, an amount u * x of resources is used. this decreases x and increases u
    // both u and v recover to their baseline levels exponentially.

    double tau_d = 200.; // ms. defines the time constant for the recovery of x (resources)
    double tau_f = 600.; // ms. defines the time constant for the recovery of u (usage)

    // when tau_d > tau_f, the neural activity is depressed
    // when tau_d < tau_f, the neural activity is facilitated

    double dx = deltaTimeMillis * ((1.0 - x) / tau_d - u * x * s);       // change for x
    double du = deltaTimeMillis * ((U - u) / tau_f + U * (1.0 - u) * s); // change for u

    double nu = u + du; // new value for u
    double nx = x + dx; // new value for x

    double w = nu * nx; // final value returned. modulates synaptic efficacy

    // update stored values
    STPu[index] = nu;
    STPx[index] = nx;

    return w;
}

void SpikingNet::decay(double deltaTime) {
    double decayConstantTimeCompensated = pow(decayConstant, deltaTime);

    for(int i = inhibitorySize; i < neuronSize; i++) {
        for(int j = inhibitorySize; j < neuronSize; j++) {
            weights[i][j] = weights[i][j] * decayConstantTimeCompensated;
        }
    }

}

// ############################### input / stimulation methods ###############################

void SpikingNet::stimulation() {
    // external input to input neurons.
    for(int i = 0; i < inputSize; ++i) {
        neurons[indexInputNeuron(i)].addToI(stimStrength);
    }
}

void SpikingNet::stimulation(double strength) {
    // external input to input neurons.
    for(int i = 0; i < inputSize; ++i) {
        neurons[indexInputNeuron(i)].addToI(strength);
    }
}

void SpikingNet::stimulation(int inputGroupIndex, double strength) {

    int group_size = floor((float) inputSize / (float) inputGroupSize);

    // external input to input neurons.
    for(int i = 0; i < inputGroupSize; ++i) {
        for(int j = (i * group_size); j<((i + 1) * group_size); ++j) {
            if(i == inputGroupIndex) {
                neurons[indexInputNeuron(j)].addToI(strength);
            }else if(i > inputGroupIndex) {
                break;
            }
        }
    }
}

void SpikingNet::wholeStimulation() {
    //    external input
    for(int i = 0; i < inputSize; ++i) {
        neurons[indexInputNeuron(i)].addToI(stimStrength);
    }
}

void SpikingNet::wholeStimulation(double strength) {
    //    external input
    for(int i = 0; i < inputSize; i++) {
        neurons[indexInputNeuron(i)].addToI(strength);
    }
}

void SpikingNet::wholeNetworkStimulation() {
    //    external input
    for(int i = 0; i < neuronSize; ++i) {
        neurons[i].addToI(stimStrength);
    }
}

void SpikingNet::wholeNetworkStimulation(double strength) {
    //    external input
    for(int i = 0; i < neuronSize; ++i) {
            neurons[i].addToI(strength);
    }
}

// ############################### Qt read / write ###############################

int SpikingNet::getNeuronSize() const {
    return neuronSize;
}

double SpikingNet::getTimeScale() const {
    return this->timeScale;
}

double SpikingNet::getInhibitoryPortion() const {
    return this->inhibitoryPortion;
}

double SpikingNet::getInputPortion() const {
    return this->inputPortion;
}

double SpikingNet::getOutputPortion() const {
    return this->outputPortion;
}

NeuronType::Enum SpikingNet::getInhibitoryNeuronType() const {
    return this->inhibitoryNeuronType;
}

NeuronType::Enum SpikingNet::getExcitatoryNeuronType() const {
    return this->excitatoryNeuronType;
}

double SpikingNet::getInhibitoryNoise() const {
    return this->inhibitoryNoise;
}

double SpikingNet::getExcitatoryNoise() const {
    return this->excitatoryNoise;
}

double SpikingNet::getSTPStrength() const {
    return this->STPStrength;
}

double SpikingNet::getSTDPStrength() const {
    return this->STDPStrength;
}

double SpikingNet::getDecayConstant() const {
    return this->decayConstant;
}

bool SpikingNet::getFlagSTP() const {
    return this->flagSTP;
}

bool SpikingNet::getFlagSTDP() const {
    return this->flagSTDP;
}

bool SpikingNet::getFlagDecay() const {
    return this->flagDecay;
}

void SpikingNet::writeNeuronSize(int neuronSize) {
    if(this->neuronSize == neuronSize)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeNeuronSize:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // reset network, since these parameters only take effect when the network is created anew
    reset();
    // do the change
    this->neuronSize = neuronSize;
    // re-initialize
    initialize();
    // signal
    emit valueChanged("neuronSize", QVariant(neuronSize));
    emit neuronSizeChanged(neuronSize);
}

void SpikingNet::writeTimeScale(double timeScale) {
    if(this->timeScale == timeScale)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeTimeScale:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->timeScale = timeScale;
    emit valueChanged("timeScale", QVariant(timeScale));
    emit timeScaleChanged(timeScale);
}

void SpikingNet::writeInhibitoryPortion(double inhibitoryPortion) {
    if(this->inhibitoryPortion == inhibitoryPortion)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeInhibitoryPortion:\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // reset network, since these parameters only take effect when the network is created anew
    // TODO: this isn't as efficient as it could be since it's not necessary to deallocate and reallocate memory for weights / STP
    reset();
    // do the change
    this->inhibitoryPortion = inhibitoryPortion;
    // re-initialize
    initialize();
    // signal
    emit valueChanged("inhibitoryPortion", QVariant(inhibitoryPortion));
    emit inhibitoryPortionChanged(inhibitoryPortion);
}

void SpikingNet::writeInputPortion(double inputPortion) {
    if(this->inputPortion == inputPortion)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeInputPortion:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // update input size
    inputSize = (neuronSize - inhibitorySize) * inputPortion;

    this->inputPortion = inputPortion;
    emit valueChanged("inputPortion", QVariant(inputPortion));
    emit inputPortionChanged(inputPortion);
}

void SpikingNet::writeOutputPortion(double outputPortion) {
    if(this->outputPortion == outputPortion)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeOutputPortion:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // update output size
    outputSize = (neuronSize - inhibitorySize) * outputPortion;

    this->outputPortion = outputPortion;
    emit valueChanged("outputPortion", QVariant(outputPortion));
    emit outputPortionChanged(outputPortion);
}

void SpikingNet::writeInhibitoryNeuronType(NeuronType::Enum inhibitoryNeuronType) {
    if(this->inhibitoryNeuronType == inhibitoryNeuronType)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeInhibitoryNeuronType:\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // reset network, since these parameters only take effect when the network is created anew
    // TODO: this isn't as efficient as it could be since it's not necessary to deallocate and reallocate memory for weights / STP
    reset();
    // do the change
    this->inhibitoryNeuronType = inhibitoryNeuronType;
    // re-initialize
    initialize();
    // signal
    emit valueChanged("inhibitoryNeuronType", QVariant(inhibitoryNeuronType));
    emit inhibitoryNeuronTypeChanged(inhibitoryNeuronType);
}

void SpikingNet::writeExcitatoryNeuronType(NeuronType::Enum excitatoryNeuronType) {
    if(this->excitatoryNeuronType == excitatoryNeuronType)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeExcitatoryNeuronType:\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    // reset network, since these parameters only take effect when the network is created anew
    // TODO: this isn't as efficient as it could be since it's not necessary to deallocate and reallocate memory for weights / STP
    reset();
    // do the change
    this->excitatoryNeuronType = excitatoryNeuronType;
    // re-initialize
    initialize();
    // signal
    emit valueChanged("excitatoryNeuronType", QVariant(excitatoryNeuronType));
    emit excitatoryNeuronTypeChanged(excitatoryNeuronType);
}

void SpikingNet::writeInhibitoryNoise(double inhibitoryNoise) {
    if(this->inhibitoryNoise == inhibitoryNoise)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeInhibitoryNoise:\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->inhibitoryNoise = inhibitoryNoise;
    emit valueChanged("inhibitoryNoise", QVariant(inhibitoryNoise));
    emit inhibitoryNoiseChanged(inhibitoryNoise);
}

void SpikingNet::writeExcitatoryNoise(double excitatoryNoise) {
    if(this->excitatoryNoise == excitatoryNoise)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeExcitatoryNoise:\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->excitatoryNoise = excitatoryNoise;
    emit valueChanged("excitatoryNoise", QVariant(excitatoryNoise));
    emit excitatoryNoiseChanged(excitatoryNoise);
}

void SpikingNet::writeSTPStrength(double STPStrength) {
    if(this->STPStrength == STPStrength)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeSTPStrength:\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->STPStrength = STPStrength;
    emit valueChanged("STPStrength", QVariant(STPStrength));
    emit STPStrengthChanged(STPStrength);
}

void SpikingNet::writeSTDPStrength(double STDPStrength) {
    if(this->STDPStrength == STDPStrength)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeSTDPStrength:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->STDPStrength = STDPStrength;
    emit valueChanged("STDPStrength", QVariant(STDPStrength));
    emit STDPStrengthChanged(STDPStrength);
}

void SpikingNet::writeDecayConstant(double decayConstant) {
    if(this->decayConstant == decayConstant)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeDecayConstant:\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->decayConstant = decayConstant;
    emit valueChanged("decayConstant", QVariant(decayConstant));
    emit decayConstantChanged(decayConstant);
}

void SpikingNet::writeFlagSTP(bool flagSTP) {
    if(this->flagSTP == flagSTP)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeFlagSTP:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->flagSTP = flagSTP;
    emit valueChanged("flagSTP", QVariant(flagSTP));
    emit flagSTPChanged(flagSTP);
}

void SpikingNet::writeFlagSTDP(bool flagSTDP) {
    if(this->flagSTDP == flagSTDP)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeFlagSTDP:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->flagSTDP = flagSTDP;
    emit valueChanged("flagSTDP", QVariant(flagSTDP));
    emit flagSTDPChanged(flagSTDP);
}

void SpikingNet::writeFlagDecay(bool flagDecay) {
    if(this->flagDecay == flagDecay)
        return;

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "writeFlagDecay:\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->flagDecay = flagDecay;
    emit valueChanged("flagDecay", QVariant(flagDecay));
    emit flagDecayChanged(flagDecay);
}
