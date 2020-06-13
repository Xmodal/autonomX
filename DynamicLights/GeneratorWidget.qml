import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import DynamicLights.Style 1.0

/**
 * Widget to control a generator.
 */
Button {
    property int generatorIndex: 1
    property string generatorName: "Generator X"
    // state props
    property bool selected: false
    property bool hovering: false

    width: parent.width
    height: 55
    layer.enabled: false

    // background
    background: Rectangle {
        color: Stylesheet.colors.black
        anchors.fill: parent
    }

    // TODO: graph

    // text content
    contentItem: RowLayout {
        spacing: 0
        anchors.leftMargin: 10
        anchors.fill: parent

        // index
        Label {
            id: labelIndex

            text: generatorIndex
            color: Stylesheet.colors.white
            font {
                family: Stylesheet.fonts.mainBold
                pointSize: 11
            }
            opacity: selected ? 1 : (hovering ? 1 : 0.5)
        }

        // generator name
        Label {
            id: labelName

            Layout.leftMargin: 30
            text: generatorName
            color: Stylesheet.colors.white
            font {
                family: selected ? Stylesheet.fonts.mainBold : Stylesheet.fonts.main
                pointSize: 18
            }
            opacity: selected ? 1 : (hovering ? 1 : 0.5)
        }
    }

    // event management
    MouseArea {
        id: btnMouseArea

        anchors.fill: parent
        hoverEnabled: true

        onClicked: selected = !selected
        onEntered: hovering = true
        onExited: hovering = false
    }
}
