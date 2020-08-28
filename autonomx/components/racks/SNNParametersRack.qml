import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"

import ca.hexagram.xmodal.autonomx 1.0

ColumnLayout {
    Layout.fillWidth: true
    spacing: 0

    SubRack {
        subRackTitle: "General"

        fields: [
            NumberField {
                labelText: "Width"
                unsigned: true
                propName: "latticeWidth"
            },

            NumberField {
                labelText: "Height"
                unsigned: true
                propName: "latticeHeight"
            },

            SliderField {
                labelText: "Time scale"
                propName: "timeScale"
                maxVal: 0.03
            }
        ]
    }

    SubRack {
        subRackTitle: "Neuron behavior"

        fields: [
            SelectField {
                labelText: "Inh. neuron type"
                propName: "inhibitoryNeuronType"
                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]
            },
            SelectField {
                labelText: "Exc. neuron type"
                propName: "excitatoryNeuronType"
                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]
            },
            SpacerField {},
            SliderField {
                labelText: "Inh. neuron noise"
                propName: "inhibitoryNoise"
                minVal: 1.0
                maxVal: 20.0
                updateLag: 70
            },
            SliderField {
                labelText: "Exc. neuron noise"
                propName: "excitatoryNoise"
                minVal: 1.0
                maxVal: 20.0
                updateLag: 70
            },
            SliderField {
                labelText: "Inh. portion"
                propName: "inhibitoryPortion"
                updateLag: 70
            }

        ]
    }

    SubRack {
        subRackTitle: "Learning"

        fields: [
            SliderField {
                labelText: "STP strength"
                propName: "STPStrength"
                flagName: "flagSTP"
                updateLag: 70
            },
            SliderField {
                labelText: "STDP strength"
                propName: "STDPStrength"
                flagName: "flagSTDP"
                updateLag: 70
            },
            SliderField {
                labelText: "Decay constant"
                propName: "decayConstant"
                flagName: "flagDecay"
                updateLag: 70
            }

        ]
    }
}
