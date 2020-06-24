import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Fields"
import "../Style"

Rack {
    id: paramsRack

    rackName: "PARAMS"
    removable: false

    RowLayout {
        Layout.fillWidth: true
        Layout.margins: Stylesheet.field.spacing
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        spacing: Stylesheet.field.spacing

        TextField {
            validateInt: true
            labelText: "Neurons"
            defaultText: "300"
        }

        SelectField {
            labelText: "Network type"
            options: ["Random", "Sparse", "Grid", "Uniform"]
        }

        SelectField {
            labelText: "Inh. neuron type"
            options: ["Spiking", "Spiking (rand)", "Resonator", "Resonator (rand)", "Chattering"]
        }

        SelectField {
            labelText: "Exc. neuron type"
            options: ["Spiking", "Spiking (rand)", "Resonator", "Resonator (rand)", "Chattering"]
        }
    }
}
