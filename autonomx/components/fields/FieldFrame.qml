import QtQuick 2.9

import "qrc:/stylesheet"

Item {
    property int frameWidth: Stylesheet.field.initialWidth
    property real maskWidth
    property bool isHovered: false
    property bool isFocused: false
    property bool backdropHovered: frameBackdrop.containsMouse
    property bool flagEnabled: false

    implicitWidth: frameWidth
    implicitHeight: 40
    anchors.fill: parent

    Item {
        anchors.right: parent.right
        anchors.fill: parent
        anchors.leftMargin: flagEnabled ? 30 : 0

        Repeater {
            id: frameLines
            model: ["top", "bottom"]
            anchors.fill: parent

            Rectangle {
                id: line

                width: parent.width
                height: 1
                anchors.top: modelData == "top" ? parent.top : undefined
                anchors.bottom: modelData == "bottom" ? parent.bottom : undefined
                color: Stylesheet.colors.white
                opacity: isHovered || isFocused || frameBackdrop.containsMouse ? 1 : 0.3

                Behavior on opacity {
                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.Linear
                    }
                }
            }
        }

        layer.enabled: true
        layer.effect: ShaderEffect {
            property real mask: maskWidth
            fragmentShader: "qrc:/shaders/slider_frame.frag"
        }
    }

    MouseArea {
        id: frameBackdrop
        anchors.fill: parent
        hoverEnabled: true
    }
}
