#ifndef NEURONTYPE_H
#define NEURONTYPE_H

#include <QObject>

class NeuronType : public QObject {
    Q_OBJECT
    Q_ENUMS(Enum);

public:
    enum Enum {
        SpikingNeuron,
        SpikingNeuronRandomized,
        ResonatorNeuron,
        ResonatorNeuronRandomized,
        ChatteringNeuron,
        InhibitoryNeuron,
        InhibitoryNeuronRandomized,
        ExcitatoryNeuron,
        ExcitatoryNeuronRandomized
    };

    explicit NeuronType(QObject *parent = 0) : QObject(parent) {
    }
};

#endif // NEURONTYPE_H
