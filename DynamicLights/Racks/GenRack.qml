import QtQuick 2.9
import QtQuick.Layouts 1.3

import "../Fields"
import "../Style"

Rack {
    id: genRack

    rackName: "GEN"
    removable: false

    RowLayout {
        Layout.fillWidth: true
        Layout.margins: Stylesheet.field.spacing
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        spacing: Stylesheet.field.spacing

        ColumnLayout {
            spacing: parent.spacing

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
            Layout.fillHeight: true
            labelText: "Description"
            placeholder: "Enter description here"
            defaultText: "This is a description for the Spiking Neural Network (SNN) generator. This algorithm creates short peaks generatively over time."

            fieldWidth: (Stylesheet.field.initialWidth * 2) + Stylesheet.field.spacing
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
