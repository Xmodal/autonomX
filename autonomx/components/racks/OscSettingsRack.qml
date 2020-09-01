import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import "qrc:/stylesheet"
import "../ui"
import "../fields"

ColumnLayout {
    Layout.fillWidth: true
    spacing: 0

    SubRack {
        subRackTitle: "In"
        columnCount: 4
        flagActive: false
        flagColor: Stylesheet.colors.inputs[0]

        fields: [
            TextField {
                labelText: "Address"
                propName: "oscInputAddress"
            },
            NumberField {
                labelText: "Port"
                propName: "oscInputPort"
            }

        ]
    }

    SubRack {
        subRackTitle: "Out"
        columnCount: 4
        flagActive: false
        flagColor: Stylesheet.colors.outputs[0]

        fields: [
            TextField {
                labelText: "Address"
                propName: "oscOutputAddressTarget"
            },
            TextField {
                labelText: "Host"
                propName: "oscOutputAddressHost"
                // TODO: input masking to only allow a standard IP address format
            },
            NumberField {
                labelText: "Port"
                propName: "oscOutputPort"
            }
        ]
    }
}
