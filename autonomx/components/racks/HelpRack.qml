import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/stylesheet"

Rack {
    id: helpRack

    rackName: "Help"
    removable: false

    content: Label {
        text: helpFiles["help_snn.html"]
        textFormat: Text.StyledText
        wrapMode: Text.WordWrap
        font.pixelSize: 14
        lineHeight: 1.15
    }
}
