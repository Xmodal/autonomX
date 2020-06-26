import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Fields"
import "../Style"

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

                defaultText: genID < 0 ? "" : generatorModel.at(genID).neuronSize
                onValueChanged: generatorModel.at(genID).neuronSize = newValue
            }

            SelectField {
                labelText: "Network type"

                options: ["Random", "Sparse", "Grid", "Uniform"]
                index: 2
            }

            SelectField {
                labelText: "Inh. neuron type"

                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]
                // index: genID < 0 ? 0 : generatorModel.at(genID).inhibitoryNeuronType
            }

            SelectField {
                labelText: "Exc. neuron type"

                options: ["Spiking", "Spiking (rand.)", "Resonator", "Resonator (rand.)", "Chattering"]
                // index: genID < 0 ? 0 : generatorModel.at(genID).excitatoryNeuronType
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: Stylesheet.field.spacing

            SliderField {
                labelText: "Inh. portion"

                currVal: genID < 0 ? 0 : generatorModel.at(genID).inhibitoryPortion
                onValueChanged: generatorModel.at(genID).inhibitoryPortion = newValue
            }

            SliderField {
                labelText: "Input portion"

                currVal: genID < 0 ? 0 : generatorModel.at(genID).inputPortion
                onValueChanged: generatorModel.at(genID).inputPortion = newValue
            }

            SliderField {
                labelText: "Output portion"

                currVal: genID < 0 ? 0 : generatorModel.at(genID).outputPortion
                onValueChanged: generatorModel.at(genID).outputPortion = newValue
            }
        }
    }
}
