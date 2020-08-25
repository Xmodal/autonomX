// === RACK VIEW ===
// this is where the generator parameters can be modified.
// parameters are stacked in "racks", each determined by their
// respective function. in complement to the essential "GEN" and "PARAMS" racks,
// additional racks can be added for further control and output management.

import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0

import "qrc:/stylesheet"
import "../components/racks"

Flickable {
    id: rackView

    contentWidth: rackContainer.width
    contentHeight: rackContainer.height

    ScrollBar.vertical: ScrollBar {
        interactive: false
        hoverEnabled: false
    }

    ColumnLayout {
        id: rackContainer
        width: 590
        spacing: 0

        Rack {
            rackName: "Generator settings"
            content: GlobalSettingsRack {}
        }

        Rack {
            rackName: "Parameters"
            content: SNNParametersRack {}
        }

        Rack {
            rackName: "OSC settings"
            content: OscSettingsRack {}
        }

        Rack {
            rackName: "Help : SNN"
            content: SNNHelpRack {}
        }
    }
}
