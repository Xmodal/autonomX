import QtQuick 2.9
import QtQuick.Layouts 1.3
import "../Fields"

Rack {
    id: genRack

    rackName: "GEN"
    removable: false

    content: RowLayout {
        Layout.fillWidth: true
        spacing: 20

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
    }
}
