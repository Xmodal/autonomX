import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import "qrc:/stylesheet"
import "../delegates"
import "../fields"

Rack {
    id: oscRack

    rackName: "OSC"

    content: ColumnLayout {
        spacing: Stylesheet.field.spacing

        RowLayout {
            spacing: Stylesheet.field.spacing
            Layout.fillWidth: true

            CheckField {
                id: enableOscIn
                on: true
                controlColor: Stylesheet.colors.input
            }

            TextField {
                labelText: "Address"
                defaultText: "/generator1/ins"
                deactivated: !enableOscIn.on
            }
            TextField {
                labelText: "Host"
                defaultText: "127.0.0.1"
                deactivated: !enableOscIn.on
            }
            NumberField {
                labelText: "Port"
                defaultNum: 57120
                deactivated: !enableOscIn.on
            }
        }

        RowLayout {
            spacing: Stylesheet.field.spacing
            Layout.fillWidth: true

            CheckField {
                id: enableOscOut
                on: true
                controlColor: Stylesheet.colors.output
            }

            TextField {
                labelText: "Address"
                defaultText: "/generator1/outs"
                deactivated: !enableOscOut.on
            }
            TextField {
                labelText: "Host"
                defaultText: "127.0.0.1"
                deactivated: !enableOscOut.on
            }
            NumberField {
                labelText: "Port"
                defaultNum: 57120
                deactivated: !enableOscOut.on
            }
        }
    }
}
