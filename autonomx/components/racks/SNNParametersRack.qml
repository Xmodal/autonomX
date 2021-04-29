import QtQuick 2.11
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
                min: 1
                max: 1000
                propName: "latticeWidth"
            },

            NumberField {
                labelText: "Height"
                min: 1
                max: 1000
                propName: "latticeHeight"
            },

            SliderField {
                labelText: "Time scale"
                propName: "timeScale"
                maxVal: 0.03
            }
        ]
    }

//    SubRack {
//        subRackTitle: "Connections"

//        fields: [
//            SelectField {
//                labelText: "Network type"
//                options: ["Grid", "Random"]
//                //propName: "networkType"
//            },
//            NumberField {
//                labelText: "Num. connections"
//                //propName: "numConnections"
//            },
//            NumberField {
//                labelText: "Learning conn."
//                //propName: "learningConnections"
//            }

//        ]
//    }

    SubRack {
        subRackTitle: "Neuron behavior"

        fields: [
            SliderField {
                labelText: "Inh. neuron noise"
                propName: "inhibitoryNoise"
                minVal: 0.0
                maxVal: 50.0
            },
            SliderField {
                labelText: "Exc. neuron noise"
                propName: "excitatoryNoise"
                minVal: 0.0
                maxVal: 50.0
            },
            SliderField {
                labelText: "Inh. portion"
                propName: "inhibitoryPortion"
                updateLag: 70
            },

            SelectField {
                labelText: "Inh. neuron type"
                propName: "inhibitoryNeuronType"
                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]
            },
            SelectField {
                labelText: "Exc. neuron type"
                propName: "excitatoryNeuronType"
                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]
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
                minVal: 0.0
                maxVal: 30.0
            },
            SliderField {
                labelText: "STDP strength"
                propName: "STDPStrength"
                flagName: "flagSTDP"
                minVal: 0.0
                maxVal: 30.0
            },
            SliderField {
                labelText: "Decay half life"
                propName: "decayHalfLife"
                flagName: "flagDecay"
                minVal: 0.1
                maxVal: 60.0
            }

        ]
    }
}
