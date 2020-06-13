// Global Rack component
// -- is always extended

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

// import stylesheet
import DynamicLights.Style 1.0

ColumnLayout {
    // metadata
    property string rackName: "Rack"
    // state flags
    property bool collapsed: false
    property bool removable: true

    // -1: no assigned generator ID
    property int genID: -1

    Layout.fillWidth: true

    // top label
    RowLayout {
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
                pointSize: 16
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
    RowLayout {}
}
