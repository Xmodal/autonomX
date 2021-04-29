import QtQuick 2.11
import QtQuick.Controls 2.2

import "qrc:/stylesheet"

Control {
    id: generatorButton

    // props
    property color backgroundColor
    property string iconSource
    property bool collapsed: false
    property string text

    signal clicked()

    font: Stylesheet.fonts.label
    clip: true

    implicitWidth: 100
    implicitHeight: 40

    enabled: state !== "hidden"

    Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors.black

        Rectangle {
            id: bg
            anchors.fill: parent
            color: backgroundColor
            opacity: mouseArea.containsMouse && !mouseArea.pressed ? 1 : 0.5
        }
    }

    Text {
        id: contentItem
        leftPadding: 15
        anchors.fill: parent
        text: generatorButton.text
        font: generatorButton.font
        color: Stylesheet.colors.white
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    Image {
        id: indicator
        source: iconSource
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.verticalCenter: parent.verticalCenter
    }

    states: State {
        name: "hidden"

        PropertyChanges { target: indicator; opacity: 0 }
        PropertyChanges { target: contentItem; opacity: 0 }
        PropertyChanges { target: generatorButton; implicitWidth: 0 }
    }

    transitions: Transition {
        NumberAnimation {
            target: generatorButton
            property: "implicitWidth"
            duration: 500
            easing.type: Easing.OutCubic
        }

        NumberAnimation {
            targets: [indicator, contentItem]
            property: "opacity"
            duration: 150
            easing.type: Easing.Linear
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onReleased: if (containsMouse) generatorButton.clicked()
    }
}
