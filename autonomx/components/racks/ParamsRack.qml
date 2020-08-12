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

            RowLayout {
                spacing: 15

                CheckField {
                    id: stpFlag

                    on: generatorModel.at(generatorIndex).flagSTP
                    onValueChanged: generatorModel.at(generatorIndex).flagSTP = newValue
                }

                SliderField {
                    labelText: "STP strength"
                    updateLag: 70

                    currVal: generatorModel.at(generatorIndex).STPStrength
                    onValueChanged: generatorModel.at(generatorIndex).STPStrength = newValue
                    deactivated: !stpFlag.on
                }
            }

            RowLayout {
                spacing: 15

                CheckField {
                    id: stdpFlag

                    on: generatorModel.at(generatorIndex).flagSTDP
                    onValueChanged: generatorModel.at(generatorIndex).flagSTDP = newValue
                }

                SliderField {
                    labelText: "STDP strength"
                    updateLag: 70

                    currVal: generatorModel.at(generatorIndex).STDPStrength
                    onValueChanged: generatorModel.at(generatorIndex).STDPStrength = newValue
                    deactivated: !stdpFlag.on
                }
            }

            RowLayout {
                spacing: 15

                CheckField {
                    id: decayFlag

                    on: generatorModel.at(generatorIndex).flagDecay
                    onValueChanged: generatorModel.at(generatorIndex).flagDecay = newValue
                }

                SliderField {
                    labelText: "Decay constant"
                    updateLag: 70

                    currVal: generatorModel.at(generatorIndex).decayConstant
                    onValueChanged: generatorModel.at(generatorIndex).decayConstant = newValue
                    deactivated: !decayFlag.on
                }
            }
        }
    }
}
