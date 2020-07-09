import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "./Style"

/**
 * Widget to control a generator.
 */
Button {
    // state props
    property bool selected: false

    width: parent.width
    height: 55
    layer.enabled: false

    // background
    background: Item {
        anchors.fill: parent

        Rectangle {
            color: selected ? Stylesheet.colors.white : Stylesheet.colors.black
            anchors.fill: parent
        }

        // index background
        Image {
            source: "assets/images/index-bg.svg"
            smooth: true
            mipmap: false
            height: parent.height
            anchors.left: parent.left
            opacity: selected ? 1 : 0
        }
    }


    // TODO: graph

    // text content
    RowLayout {
        spacing: 0
        anchors.leftMargin: 10
        anchors.fill: parent
        Layout.alignment: Qt.AlignVCenter

        // index
        Label {
            id: labelIndex

            text: model.index + 1
            color: Stylesheet.colors.white
            font {
                family: Stylesheet.fonts.main
                weight: Font.Bold
                pixelSize: 11
            }
            opacity: selected ? 1 : (hovered ? 1 : 0.5)
        }

        // generator name
        Label {
            id: labelName

            Layout.leftMargin: 30
            text: model.name
            color: selected ? Stylesheet.colors.black : Stylesheet.colors.white
            font {
                family: Stylesheet.fonts.main
                weight: Font.Normal
                pixelSize: 18
            }
            opacity: selected ? 1 : (hovered ? 1 : 0.5)
        }

        // output indicator
        OutputIndicator {
            id: outputIndicator

            luminosity: model.outputMonitor
            lightColor: Stylesheet.colors.outputs[model.index % Stylesheet.colors.outputs.length]

            Layout.rightMargin: 10
            Layout.alignment: Qt.AlignRight
        }
    }

    // inferior border
    Rectangle {
        id: borderBottom
        width: parent.width
        height: 1
        anchors.bottom: parent.bottom
        color: Stylesheet.colors.white
        opacity: 0.1
    }

    onClicked: selected = !selected
}
