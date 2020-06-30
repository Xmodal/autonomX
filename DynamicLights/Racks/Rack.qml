// Global Rack component
// -- is always extended

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Style"
import "../Fields"

ColumnLayout {
    // metadata
    property string rackName: "Rack"
    property int genID: -1 // -1: no assigned generator ID
    // state flags
    property bool collapsed: false
    property bool removable: true

    Layout.fillWidth: true
    Layout.maximumHeight: 245
    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    spacing: 0

    // top label
    RowLayout {
        Layout.fillWidth: true

        Label {
            id: rackTitle
            text: rackName

            leftPadding: 20
            bottomPadding: 10
            topPadding: 10
            Layout.fillWidth: true

            font {
                family: Stylesheet.fonts.sub
                pixelSize: 16
                letterSpacing: 0.8
                capitalization: Font.Capitalize
            }

            background: Rectangle {
                color: "#717171"
            }
        }

        //TODO: collapse rack button + state logic
        //Button { id: btnCollapse }
    }

    // content (extended)
}
