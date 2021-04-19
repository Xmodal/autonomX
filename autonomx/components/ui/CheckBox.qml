import QtQuick 2.9
import QtQuick.Controls 2.3

import "qrc:/stylesheet"

CheckBox {
    id: control

    property color controlColor: Stylesheet.colors.white
    property real scaleValue: checked ? (hovered ? 1.2 : 1) : 0
    property real size: 20

    implicitWidth: size
    implicitHeight: size

    background: Rectangle {
        id: controlFrame
        anchors.fill: parent
        color: "transparent"
        border {
            width: 1
            color: Stylesheet.colors.white
        }
        opacity: control.hovered || control.checked ? 1 : 0.5

        Behavior on opacity {
            NumberAnimation {
                property: "opacity"
                duration: 150
                easing.type: Easing.Linear
            }
        }
    }

    indicator: Rectangle {
        id: indicator
        anchors.centerIn: parent
        width: size / 2; height: size / 2
        color: controlColor
        antialiasing: true

        transform: Scale {
            xScale: scaleValue
            yScale: scaleValue
            origin {
                x: indicator.x
                y: indicator.y
            }
        }
    }

    CursorShaper {}

    Behavior on scaleValue {
        NumberAnimation { duration: 350; easing.type: Easing.OutQuart }
    }
}
