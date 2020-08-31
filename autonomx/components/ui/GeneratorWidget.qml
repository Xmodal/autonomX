import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

/**
 * Widget to control a generator.
 */
Button {
    id: generatorWidget

    // state props
    property bool selected: index === window.activeGeneratorIndex

    // autofocus on selected
    onSelectedChanged: {
        if (selected) {
            forceActiveFocus();
            generatorList.positionViewAtIndex(window.activeGeneratorIndex, ListView.SnapPosition)
        }
    }

    // dimensions
    implicitWidth: parent ? parent.width : 0
    implicitHeight: 55

    // delete shortcut
    Keys.onPressed: {
        if (activeFocus && selected && event.key === Qt.Key_Delete && generatorModel.rowCount() > 1) window.deleteGenerator()
    }

    // background
    background: Rectangle {
        color: selected ? Stylesheet.colors.generator : Stylesheet.colors.black
        anchors.fill: parent

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

        historyLatest: model.historyLatest
        historyRefresher: model.historyRefresher

        strokeColor: selected ? Stylesheet.colors.white : Stylesheet.colors.generator
    }

    // text content
    RowLayout {
        spacing: 0
        anchors.fill: parent

        // index
        Label {
            id: labelIndex

            Layout.leftMargin: 12
            Layout.preferredWidth: 10

            text: model.index + 1
            color: Stylesheet.colors[selected ? 'darkGrey' : 'white']
            font { weight: Font.DemiBold; pixelSize: 11 }
            opacity: selected ? 1 : (hovered ? 1 : 0.5)
            horizontalAlignment: Text.AlignHCenter
        }

        // generator name
        Label {
            id: labelName

            text: model.name
            Layout.leftMargin: 30
            Layout.fillWidth: true
            color: Stylesheet.colors.white
            font.weight: Font.DemiBold
            opacity: selected ? 1 : (hovered ? 1 : 0.5)
        }

        // VU-meter
        VuMeter {
            id: vuMeter

            intensity: model.historyLatest
            barColor: Stylesheet.colors.white

            Layout.alignment: Qt.AlignRight
        }
    }

    // delete button
    Button {
        id: deleteButton

        anchors.left: parent.left
        anchors.leftMargin: generatorList.count > 1 ? (hovered ? 0 : (generatorWidget.hovered ? -25 : -35)) : -35
        implicitWidth: 35
        implicitHeight: parent.height
        z: 1
        enabled: generatorList.count > 1

        Behavior on anchors.leftMargin {
            NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
        }

        background: Rectangle {
            anchors.fill: parent
            color: Stylesheet.colors.black
            antialiasing: true

            Rectangle {
                anchors.fill: parent
                color: Stylesheet.colors.cancel
                opacity: deleteButton.pressed ? 0.7 : 1
                antialiasing: true
            }
        }

        indicator: Image {
            anchors.centerIn: parent
            source: "qrc:/assets/images/delete-icon.svg"
        }

        onClicked: window.deleteGenerator(index)
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
