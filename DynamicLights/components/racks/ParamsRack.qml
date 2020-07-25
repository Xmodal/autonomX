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
        ColumnLayout {
            Layout.fillWidth: true
            spacing: Stylesheet.field.spacing

            RowLayout {
                Layout.fillWidth: true
                spacing: Stylesheet.field.spacing

                NumberField {
                    labelText: "Neurons"
                    unsigned: true

                    defaultNum: generatorModel.at(genID).neuronSize
                    onValueChanged: generatorModel.at(genID).neuronSize = newValue
                }

                SliderField {
                    labelText: "Time scale"

                    currVal: generatorModel.at(genID).timeScale
                    onValueChanged: generatorModel.at(genID).timeScale = newValue
                }
            }
            RowLayout {
                Layout.fillWidth: true
                spacing: Stylesheet.field.spacing

                SliderField {
                    labelText: "Inh. portion"

                    updateLag: 70

                    currVal: generatorModel.at(genID).inhibitoryPortion
                    onValueChanged: generatorModel.at(genID).inhibitoryPortion = newValue
                }

                SliderField {
                    labelText: "Input portion"

                    updateLag: 70

                    currVal: generatorModel.at(genID).inputPortion
                    onValueChanged: generatorModel.at(genID).inputPortion = newValue
                }

                SliderField {
                    labelText: "Output portion"

                    updateLag: 70

                    currVal: generatorModel.at(genID).outputPortion
                    onValueChanged: generatorModel.at(genID).outputPortion = newValue
                }
            }
        }

        // --= NEURON BEHAVIOR =--
        RowLayout {
            Layout.fillWidth: true
            spacing: Stylesheet.field.spacing

            SelectField {
                labelText: "Inh. neuron type"

                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]

                index: generatorModel.at(genID).inhibitoryNeuronType
                onValueChanged: generatorModel.at(genID).inhibitoryNeuronType = newValue
            }

            SliderField {
                labelText: "Inh. neuron noise"

                minVal: 1.0
                maxVal: 20.0
                updateLag: 70

                currVal: generatorModel.at(genID).inhibitoryNoise
                onValueChanged: generatorModel.at(genID).inhibitoryNoise = newValue
            }

            SelectField {
                labelText: "Exc. neuron type"

                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]

                index: generatorModel.at(genID).excitatoryNeuronType
                onValueChanged: generatorModel.at(genID).excitatoryNeuronType = newValue
            }

            SliderField {
                labelText: "Exc. neuron noise"

                minVal: 1.0
                maxVal: 20.0
                updateLag: 70

                currVal: generatorModel.at(genID).excitatoryNoise
                onValueChanged: generatorModel.at(genID).excitatoryNoise = newValue
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

                    on: generatorModel.at(genID).flagSTP
                    onValueChanged: generatorModel.at(genID).flagSTP = newValue
                }

                SliderField {
                    labelText: "STP strength"
                    updateLag: 70

                    currVal: generatorModel.at(genID).STPStrength
                    onValueChanged: generatorModel.at(genID).STPStrength = newValue
                    deactivated: !stpFlag.on
                }
            }

            RowLayout {
                spacing: 15

                CheckField {
                    id: stdpFlag

                    on: generatorModel.at(genID).flagSTDP
                    onValueChanged: generatorModel.at(genID).flagSTDP = newValue
                }

                SliderField {
                    labelText: "STDP strength"
                    updateLag: 70

                    currVal: generatorModel.at(genID).STDPStrength
                    onValueChanged: generatorModel.at(genID).STDPStrength = newValue
                    deactivated: !stdpFlag.on
                }
            }

            RowLayout {
                spacing: 15

                CheckField {
                    id: decayFlag

                    on: generatorModel.at(genID).flagDecay
                    onValueChanged: generatorModel.at(genID).flagDecay = newValue
                }

                SliderField {
                    labelText: "Decay constant"
                    updateLag: 70

                    currVal: generatorModel.at(genID).decayConstant
                    onValueChanged: generatorModel.at(genID).decayConstant = newValue
                    deactivated: !decayFlag.on
                }
            }
        }
    }
}
