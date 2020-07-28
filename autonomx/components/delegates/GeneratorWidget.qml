import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

/**
 * Widget to control a generator.
 */
Button {
    // state props
    property bool selected: false

    width: parent ? parent.width : 0
    height: 55
    layer.enabled: false

    // background
    background: Item {
        anchors.fill: parent

        Rectangle {
            color: selected ? Stylesheet.colors.generator : Stylesheet.colors.black
            anchors.fill: parent
        }

        // index background
        Image {
            source: "qrc:/assets/images/index-bg.svg"
            height: parent.height
            anchors.left: parent.left
            opacity: selected ? 1 : 0
        }
    }


    // TODO: graph
    HistoryGraph {
        id: historyGraph

        newValue: model.index >= 0 ? model.outputMonitor : 0

        strokeColor: model.index >= 0 ? (selected ? Stylesheet.colors.generator : Stylesheet.colors.darkGrey) : "#000"
    }

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
            color: Stylesheet.colors[selected ? 'darkGrey' : 'white']
            font {
                weight: Font.DemiBold
                pixelSize: 11
            }
            opacity: selected ? 1 : (hovered ? 1 : 0.5)
        }

        // generator name
        Label {
            id: labelName

            Layout.leftMargin: 30
            text: model.name
            color: Stylesheet.colors.white
            font {
                weight: Font.DemiBold
            }
            opacity: selected ? 1 : (hovered ? 1 : 0.5)
        }

        // output indicator
        OutputIndicator {
            id: outputIndicator

            luminosity: model.index >= 0 ? model.outputMonitor : 0
            lightColor:  model.index >= 0 ? Stylesheet.colors.generator : "#000"

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
