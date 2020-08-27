import QtQuick 2.9
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"

Flow {
    spacing: Stylesheet.field.spacing

    TextField {
        labelText: "Name"
        placeholder: "Name"
        propName: "name"
    }

    SelectField {
        labelText: "Type"
        options: ["SNN"]
        // TODO: link Generator property
    }

    AreaField {
        Layout.fillHeight: true
        labelText: "Description"
        placeholder: "Enter description here"
        propName: "description"
        fieldWidth: (Stylesheet.field.initialWidth * 2) + Stylesheet.field.spacing
    }
}
