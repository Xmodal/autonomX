import QtQuick 2.9
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"

RowLayout {
    spacing: Stylesheet.field.spacing

    ColumnLayout {
        spacing: parent.spacing

        TextField {
            labelText: "Name"
            placeholder: "Name"

            defaultText: generatorModel.at(generatorIndex).name
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

        defaultText: generatorModel.at(generatorIndex).description
        onValueChanged: generatorModel.at(generatorIndex).description = newValue

        fieldWidth: (Stylesheet.field.initialWidth * 2) + Stylesheet.field.spacing
    }
}
