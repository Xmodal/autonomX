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

// this is probably a bad include to have here. is there a better way to setup debug output that plays well with Qt? qDebug doesn't work here for some reason
#include <iostream>

// ############################### initialization routines ###############################

SpikingNet::SpikingNet() {
    // default values for descriptive properties
    name = "Spiking Neural Network";
    type = "SNN";
    description = "An interconnected network of biologically-modeled neurons.";
    outputMonitor = 0;

    STDPWindow = 20;

    // set random seed
    if(flagRandomDevice)
    {
        std::random_device random;
        randomGenerator.seed(random());
    }
    else
    {
        randomGenerator.seed(randomSeed);
    }


    neurons.resize(neuronSize);

    outputSpiking.resize(outputGroupSize, 0);

    STDPTimes.resize(neuronSize, 0);

    // STP (according to Science paper)
    STPu = new double[neuronSize];
    STPx = new double[neuronSize];
    STPw = new double[neuronSize];

    weights = new double*[neuronSize];
    for(int i = 0; i < neuronSize; ++i) {
        weights[i] = new double[neuronSize];
    }

    // setup generator inputs and outputs
    input.resize(inputGroupSize, 0.0);
    output.resize(outputGroupSize, 0.0);
}

SpikingNet::~SpikingNet() {

    // delete STP variables
    delete[] STPu;
    delete[] STPx;
    delete[] STPw;
    STPu = 0;
    STPx = 0;
    STPw = 0;

    for(int i = 0; i < neuronSize; ++i) {
        delete[] weights[i];
        weights[i] = 0;
    }
    delete[] weights;
    weights = 0;
}


void SpikingNet::init() {

    for(int i = 0; i < neuronSize; ++i) {
        for(int j = 0; j < neuronSize; ++j) {
            weights[i][j] = 0.;
        }
    }

    neurons.clear();
    STDPTimes.clear();

    neurons.resize(neuronSize);

    outputSpiking.resize(outputGroupSize, 0);

    STDPTimes.resize(neuronSize, 0);

    // initialization of some variables
    for(int i = 0; i < neuronSize; ++i) {
        // initialize STDP variables
        STDPTimes[i] = 0;

        // initialize STP variables
        STPw[i] = 1.0;
        STPx[i] = 1.0;
        STPu[i] = 0.0;
    }

    // set neuron types
    for(int i = 0; i < neuronSize; ++i) {
        if(i < inhibitorySize) {
            neurons[i].setNeuronType(inhibitoryNeuronType);
        }else{
            neurons[i].setNeuronType(excitatoryNeuronType);
        }
    }

    // set network type
    switch(networkType) {
        case sparseNetwork:
            setSparseNetwork();
            break;
        case randomNetwork:
            setRandomNetwork();
            break;
        case uniformNetwork: // for debug
            setUniformNetwork();
            break;
        case gridNetwork:
            setGridNetwork();
            break;
    }
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

    printf("sum of connections: %d\n", connectionSum);
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
    printf("sum of connections: %d\n", connectionSum);
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
    printf("sum of connections: %d\n", connectionSum);

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
    // apply input stimulation
    for(int i = 0; i < inputGroupSize; i++) {
        stimulation(i, input[i]);
    }

    // update, applying time scale
    update(deltaTime * timeScale);

    // write output
    for(int i = 0; i < outputGroupSize; i++) {
        output[i] = getSpikedOutput(i);
    }

}

void SpikingNet::update(double deltaTime) {

    if(flagDecay) decay(deltaTime);
    if(flagSTDP)  computeSTDP(deltaTime);
    if(flagSTP)   computeSTP(deltaTime);

    updateInput();
    updateNeurons(deltaTime);

    checkFiring();
}

void SpikingNet::checkFiring() {
    // reset spiking output
    for(int i = 0; i < outputGroupSize; i++) {
        outputSpiking[i] = 0;
    }
    int sizePerGroup = (outputSize / outputGroupSize);
    int total = 0;
    for(int i = 0; i < outputSize; i++) {
        // get the index of the output neuron
        int index = indexOutputNeuron(i);
        // check to see if the neuron is firing
        if(neurons[index].applyFiring()) {
            total++;
            // get the index of the group this neuron belongs to. min is there to prevent an edge case where the number of group doesn't perfectly divide the number of output neurons.
            int indexGroup = std::min<int>(i / sizePerGroup, outputGroupSize - 1);
            // apply increment with averaging. this isn't perfect if the last group size isn't exactly the same as the other ones, but probably shouldn't be a big problem.
            outputSpiking[indexGroup] += 1.0 / sizePerGroup;
        }
    }
    if(flagDebug) {
        std::cout << "number of neurons firing: " << total << endl;
    }
}


// returns the per-output group spiking average
int SpikingNet::getSpikedOutput(int index) {
    return outputSpiking[index];
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
                    d = deltaTimeMillis * (0.1 * pow(0.95, (1000.0 * STDPTimes[j])));

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
        STPw[i] = getSTPValue(i, neurons[i].isFiring(), deltaTime);
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