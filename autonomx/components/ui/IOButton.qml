import QtQuick 2.9
import QtQuick.Controls 2.9

import "qrc:/stylesheet"

Button {
    property int type: 0                    // none = -1; input = 0; output = 1

    readonly property color buttonColor: type < 0 ? Stylesheet.colors.white : Stylesheet.colors[type ? "outputs" : "inputs"][0]
    readonly property color activeTextColor: Stylesheet.colors[type ? "white" : "black"]

    font: Stylesheet.fonts.label
    text: type ? 'O' : 'I'
    opacity: hovered || checked ? 1 : 0.35

    implicitWidth: 20
    implicitHeight: 20
    padding: 0

    contentItem: Label {
        text: parent.text
        font: parent.font
        color: (pressed || checked) ? activeTextColor : buttonColor
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
    }

    background: Rectangle {
        color: (pressed || checked) ? buttonColor : "transparent"

        border {
            color: buttonColor
            width: 1
        }
    }

    CursorShaper {}
}
