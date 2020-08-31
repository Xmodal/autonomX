import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "qrc:/stylesheet"

ColumnLayout {
    Layout.fillWidth: true

    SubRack {
        titleBarVisible: false

        Label {
            Layout.fillWidth: true
            leftPadding: 30
            rightPadding: 50
            topPadding: 20
            bottomPadding: 30

            text: helpFiles["help_snn.html"]
            textFormat: Text.StyledText
            wrapMode: Text.WordWrap
            font.pixelSize: 14
            lineHeight: 1.15
        }
    }
}
