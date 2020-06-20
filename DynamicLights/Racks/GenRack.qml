import QtQuick 2.9
import QtQuick.Layouts 1.3
import "../Fields"

Rack {
    id: genRack

    rackName: "GEN"
    removable: false

    content: RowLayout {
        Layout.fillWidth: true
        spacing: 30

        ColumnLayout {
            spacing: 30;

            TextField {
                labelText: "Name"
                placeholder: "Name"
                defaultText: "Spiking Neural Network"
            }

            SelectField {
                labelText: "Type"
                options: ["SNN"]
            }
        }

        AreaField {
            labelText: "Description"
            placeholder: "Enter description here"
            defaultText: "This is a description for the Spiking Neural Network (SNN) generator. This algorithm creates short peaks generatively over time."

            fieldWidth: 400
            height: parent.height
        }
    }
}
