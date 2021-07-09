// === RACK VIEW ===
// this is where the generator parameters can be modified.
// parameters are stacked in "racks", each determined by their
// respective function. in complement to the essential "GEN" and "PARAMS" racks,
// additional racks can be added for further control and output management.

import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../components/racks"

Rectangle {
    Layout.preferredWidth: 580
    Layout.fillHeight: true

    color: Stylesheet.colors.darkGrey

    Flickable {
        id: rackView

        width: parent.width
        height: parent.height
        contentHeight: rackContainer.height

        ScrollBar.vertical: ScrollBar {
            interactive: false
            hoverEnabled: false
        }

        ColumnLayout {
            id: rackContainer
            width: parent.width
            spacing: 0

            Rack {
                rackName: "Generator info"
                content: GlobalSettingsRack {}
            }

            Rack {
                rackName: "Parameters"
                content: ParametersRack {}
            }

//            Rack {
//                rackName: "OSC settings"
//                content: OscSettingsRack {}
//            }
        }
    }
}
