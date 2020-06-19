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

    // components
    property alias content: contentLoader.sourceComponent

    Layout.fillWidth: true
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

            color: Stylesheet.colors.white
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

    // content (todo)
    Item {
        Layout.fillWidth: true

        Rectangle {
            anchors.fill: parent
            color: "#212121"
            border.width: 0
        }

        RowLayout {
            anchors.leftMargin: 20
            anchors.bottomMargin: 40
            anchors.rightMargin: 20
            anchors.topMargin: 40
            anchors.fill: parent
            spacing: 0

            // field content
            Loader { id: contentLoader }
        }
    }

}
