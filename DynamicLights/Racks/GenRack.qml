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

                // TODO: softcode this
                // ("softcode" loosely defined here as the opposite of "hardcode")
                defaultText: genID < 0 ? "" : generatorModel.at(genID).name
                onValueChanged: generatorModel.at(genID).name = newValue
            }

            SelectField {
                labelText: "Type"
                options: ["SNN"]
                // TODO: link Generator property
            }
        }

        AreaField {
            Layout.fillHeight: true
            labelText: "Description"
            placeholder: "Enter description here"

            defaultText: genID < 0 ? "" : generatorModel.at(genID).description
            onValueChanged: generatorModel.at(genID).description = newValue

            fieldWidth: (Stylesheet.field.initialWidth * 2) + Stylesheet.field.spacing
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
