import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"

import ca.hexagram.xmodal.autonomx 1.0

Rack {
    id: paramsRack

    rackName: "PARAMS"
    removable: false

    content: ColumnLayout {
        // TEMPORARY MEASURE, WILL REMOVE IN NEXT SPRINT
        spacing: 50

        // --= BASIC =--
        RowLayout {
            Layout.fillWidth: true
            spacing: Stylesheet.field.spacing

            NumberField {
                labelText: "Width"
                unsigned: true

                defaultNum: generatorModel.at(generatorIndex).latticeWidth
                onValueChanged: generatorModel.at(generatorIndex).latticeWidth = newValue
            }

            NumberField {
                labelText: "Height"
                unsigned: true

                defaultNum: generatorModel.at(generatorIndex).latticeHeight
                onValueChanged: generatorModel.at(generatorIndex).latticeHeight = newValue
            }

            SliderField {
                labelText: "Time scale"

                currVal: generatorModel.at(generatorIndex).timeScale
                onValueChanged: generatorModel.at(generatorIndex).timeScale = newValue
            }

            SliderField {
                labelText: "Inh. portion"

                updateLag: 70

                currVal: generatorModel.at(generatorIndex).inhibitoryPortion
                onValueChanged: generatorModel.at(generatorIndex).inhibitoryPortion = newValue
            }
        }

        // --= NEURON BEHAVIOR =--
        RowLayout {
            Layout.fillWidth: true
            spacing: Stylesheet.field.spacing

            SelectField {
                labelText: "Inh. neuron type"

                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]

                index: generatorModel.at(generatorIndex).inhibitoryNeuronType
                onValueChanged: generatorModel.at(generatorIndex).inhibitoryNeuronType = newValue
            }

            SliderField {
                labelText: "Inh. neuron noise"

                minVal: 1.0
                maxVal: 20.0
                updateLag: 70

                currVal: generatorModel.at(generatorIndex).inhibitoryNoise
                onValueChanged: generatorModel.at(generatorIndex).inhibitoryNoise = newValue
            }

            SelectField {
                labelText: "Exc. neuron type"

                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]

                index: generatorModel.at(generatorIndex).excitatoryNeuronType
                onValueChanged: generatorModel.at(generatorIndex).excitatoryNeuronType = newValue
            }

            SliderField {
                labelText: "Exc. neuron noise"

                minVal: 1.0
                maxVal: 20.0
                updateLag: 70

                currVal: generatorModel.at(generatorIndex).excitatoryNoise
                onValueChanged: generatorModel.at(generatorIndex).excitatoryNoise = newValue
            }
        }

        // --= FLAGS =--
        RowLayout {
            Layout.fillWidth: true
            spacing: Stylesheet.field.spacing

            SliderField {
                labelText: "STP strength"
                updateLag: 70

                // value
                currVal: generatorModel.at(generatorIndex).STPStrength
                onValueChanged: generatorModel.at(generatorIndex).STPStrength = newValue

                // flag
                enableFlag: true
                flagValue: generatorModel.at(generatorIndex).flagSTP
                onFlagChanged: generatorModel.at(generatorIndex).flagSTP = newFlag
            }

            SliderField {
                labelText: "STDP strength"
                updateLag: 70

                // value
                currVal: generatorModel.at(generatorIndex).STDPStrength
                onValueChanged: generatorModel.at(generatorIndex).STDPStrength = newValue

                // flag
                enableFlag: true
                flagValue: generatorModel.at(generatorIndex).flagSTDP
                onFlagChanged: generatorModel.at(generatorIndex).flagSTDP = newFlag
            }

            SliderField {
                labelText: "Decay constant"
                updateLag: 70

                // value
                currVal: generatorModel.at(generatorIndex).decayConstant
                onValueChanged: generatorModel.at(generatorIndex).decayConstant = newValue

                // flag
                enableFlag: true
                flagValue: generatorModel.at(generatorIndex).flagDecay
                onFlagChanged: generatorModel.at(generatorIndex).flagDecay = newFlag
            }
        }
    }
}
