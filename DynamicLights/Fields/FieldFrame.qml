import QtQuick 2.9

import "../Style"

Item {
    property int frameWidth: Stylesheet.field.initialWidth
    property bool isHovered: false
    property bool isFocused: false

    implicitWidth: frameWidth
    implicitHeight: 40
    anchors.fill: parent

    Repeater {
        id: frameLines
        model: ["top", "bottom"]

        Rectangle {
            id: line
            width: parent.width
            height: 1
            anchors.top: modelData == "top" ? parent.top : undefined
            anchors.bottom: modelData == "bottom" ? parent.bottom : undefined
            color: Stylesheet.colors.white
            opacity: 0.5

            // states
            state: isHovered ? "active" : (isFocused ? "active" : "")
            states: State {
                name: "active"
                PropertyChanges { target: line; opacity: 1 }
            }

            // animations
            Behavior on opacity {
                NumberAnimation {
                    property: "opacity"
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
