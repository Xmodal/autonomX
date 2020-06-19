import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "./Style"

/**
 * Widget to control a generator.
 */
Button {
    property int generatorIndex: 1
    property string generatorName: "Default Name"
    property string generatorType: "Default Type"
    property string generatorDescription: "Default description"
    property double generatorOutputMonitor: 0.0

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
                family: Stylesheet.fonts.main
                weight: Font.Bold
                pixelSize: 11
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
                family: Stylesheet.fonts.main
                weight: selected ? Font.Bold : Font.Normal
                pixelSize: 18
            }
            opacity: selected ? 1 : (hovering ? 1 : 0.5)
        }

        // output indicator
        OutputIndicator {
            id: outputIndicator

            luminosity: generatorOutputMonitor

            Layout.rightMargin: 10
            Layout.alignment: Qt.AlignRight
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
