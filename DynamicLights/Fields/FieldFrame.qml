import QtQuick 2.9

import "../Style"

Item {
    property int frameWidth: 200
    property bool isHovered: false
    property bool isFocused: false

    implicitWidth: frameWidth

    Repeater {
        model: ["top", "bottom"]
        Rectangle {
            width: parent.width
            height: 1
            anchors.top: modelData == "top" ? parent.top : undefined
            anchors.bottom: modelData == "bottom" ? parent.bottom : undefined
            color: Stylesheet.colors.white
            opacity: isHovered ? 1 : (isFocused ? 1 : 0.5)
        }
    }
}
