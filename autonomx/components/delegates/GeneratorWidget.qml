import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

/**
 * Widget to control a generator.
 */
Button {
    // state props
    property bool selected: index === window.activeGeneratorIndex

    width: parent ? parent.width : 0
    height: 55
    layer.enabled: false

    // delete shortcut
    Keys.onPressed: {
        if (activeFocus && selected && event.key === Qt.Key_Delete && generatorList.count > 1) appModel.deleteGenerator(generatorModel.at(window.activeGeneratorIndex).id)
    }

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


    // history graph
    // TODO: refactor into OpenGL component
    HistoryGraph {
        id: historyGraph

        newValue: model.outputMonitor
        strokeColor: selected ? Stylesheet.colors.white : Stylesheet.colors.generator
    }

    // text content
    RowLayout {
        spacing: 0
        anchors.leftMargin: 12
        anchors.fill: parent
        Layout.alignment: Qt.AlignVCenter

        // index
        Label {
            id: labelIndex

            width: 10
            text: model.index + 1
            color: Stylesheet.colors[selected ? 'darkGrey' : 'white']
            font { weight: Font.DemiBold; pixelSize: 11 }
            opacity: selected ? 1 : (hovered ? 1 : 0.5)
            horizontalAlignment: Text.AlignHCenter
        }

        // generator name
        Label {
            id: labelName

            Layout.leftMargin: 30
            text: model.name
            color: Stylesheet.colors.white
            font.weight: Font.DemiBold
            opacity: selected ? 1 : (hovered ? 1 : 0.5)
        }

        // VU-meter
        VuMeter {
            id: vuMeter

            intensity: model.outputMonitor
            barColor: Stylesheet.colors.white

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
}
