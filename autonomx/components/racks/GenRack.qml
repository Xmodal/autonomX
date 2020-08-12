import QtQuick 2.9
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"

Rack {
    id: genRack

    rackName: "GEN"
    removable: false

    content: RowLayout {
        spacing: Stylesheet.field.spacing

        ColumnLayout {
            spacing: parent.spacing

            TextField {
                labelText: "Name"
                placeholder: "Name"

                // TODO: softcode this
                // ("softcode" loosely defined here as the opposite of "hardcode")
                defaultText: generatorIndex < 0 ? "" : generatorModel.at(generatorIndex).name
                onValueChanged: generatorModel.at(generatorIndex).name = newValue
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

            defaultText: generatorIndex < 0 ? "" : generatorModel.at(generatorIndex).description
            onValueChanged: generatorModel.at(generatorIndex).description = newValue

            fieldWidth: (Stylesheet.field.initialWidth * 2) + Stylesheet.field.spacing
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
