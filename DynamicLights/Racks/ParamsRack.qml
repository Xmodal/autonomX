import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Fields"
import "../Style"

import com.dynamiclights 1.0

Rack {
    id: paramsRack

    rackName: "PARAMS"
    removable: false

    ColumnLayout {
        Layout.fillWidth: true
        Layout.margins: Stylesheet.field.spacing
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        spacing: Stylesheet.field.spacing

        RowLayout {
            Layout.fillWidth: true
            spacing: Stylesheet.field.spacing

            TextField {
                validateInt: true
                labelText: "Neurons"

                defaultText: generatorModel.at(genID).neuronSize
                onValueChanged: generatorModel.at(genID).neuronSize = newValue
            }

//            SelectField {
//                labelText: "Network type"

//                options: ["Random", "Sparse", "Uniform", "Grid"]
//                // TODO: make singletons for these
//                enumOptions: [
//                    SpikingNet.RandomNetwork,
//                    SpikingNet.SparseNetwork,
//                    SpikingNet.UniformNetwork,
//                    SpikingNet.GridNetwork
//                ]
//                index: generatorModel.at(genID).networkType
//                onValueChanged: generatorModel.at(genID).networkType = enumOptions[newValue]
//            }

            SelectField {
                labelText: "Inh. neuron type"

                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]
            }

            SelectField {
                labelText: "Exc. neuron type"

                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: Stylesheet.field.spacing

            SliderField {
                labelText: "Inh. portion"

                updateLag: 70

                currVal: genID < 0 ? 0 : generatorModel.at(genID).inhibitoryPortion
                onValueChanged: generatorModel.at(genID).inhibitoryPortion = newValue
            }

            SliderField {
                labelText: "Input portion"

                updateLag: 70

                currVal: genID < 0 ? 0 : generatorModel.at(genID).inputPortion
                onValueChanged: generatorModel.at(genID).inputPortion = newValue
            }

            SliderField {
                labelText: "Output portion"

                updateLag: 70

                currVal: genID < 0 ? 0 : generatorModel.at(genID).outputPortion
                onValueChanged: generatorModel.at(genID).outputPortion = newValue
            }
        }
    }
}
