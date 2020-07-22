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
                on: true
                controlColor: Stylesheet.colors.input
            }

            TextField {
                labelText: "Address"
                defaultText: "/generator1/ins"
            }
            TextField {
                labelText: "Host"
                defaultText: "127.0.0.1"
            }
            NumberField {
                labelText: "Port"
                defaultNum: 57120
            }
        }

        RowLayout {
            spacing: Stylesheet.field.spacing
            Layout.fillWidth: true

            CheckField {
                on: true
                controlColor: Stylesheet.colors.output
            }

            TextField {
                labelText: "Address"
                defaultText: "/generator1/outs"
            }
            TextField {
                labelText: "Host"
                defaultText: "127.0.0.1"
            }
            NumberField {
                labelText: "Port"
                defaultNum: 57120
            }
        }
    }
}
