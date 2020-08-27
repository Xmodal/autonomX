import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"

import ca.hexagram.xmodal.autonomx 1.0

Flow {
    Layout.fillWidth: true
    spacing: Stylesheet.field.spacing

    NumberField {
        labelText: "Width"
        unsigned: true
        propName: "latticeWidth"
    }

    NumberField {
        labelText: "Height"
        unsigned: true
        propName: "latticeHeight"
    }

    SliderField {
        labelText: "Time scale"
        propName: "timeScale"
    }

    SliderField {
        labelText: "Inh. portion"
        propName: "inhibitoryPortion"
        updateLag: 70
    }

    SliderField {
        labelText: "Inh. neuron noise"
        propName: "inhibitoryNoise"
        minVal: 1.0
        maxVal: 20.0
        updateLag: 70
    }

    SliderField {
        labelText: "Exc. neuron noise"
        propName: "excitatoryNoise"
        minVal: 1.0
        maxVal: 20.0
        updateLag: 70
    }

    SpacerField {}

    SelectField {
        labelText: "Inh. neuron type"
        propName: "inhibitoryNeuronType"
        options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]
    }

    SelectField {
        labelText: "Exc. neuron type"
        propName: "excitatoryNeuronType"
        options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]
    }

    SliderField {
        labelText: "STP strength"
        updateLag: 70
        propName: "STPStrength"
        flagName: "flagSTP"
    }

    SliderField {
        labelText: "STDP strength"
        updateLag: 70
        propName: "STDPStrength"
        flagName: "flagSTDP"
    }

    SliderField {
        labelText: "Decay constant"
        updateLag: 70
        propName: "decayConstant"
        flagName: "flagDecay"
    }
}
