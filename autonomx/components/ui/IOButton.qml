import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/stylesheet"

Button {
    property int type: 0        // input = 0; output = 1

    readonly property color buttonColor: Stylesheet.colors[type === 0 ? "inputs" : "outputs"][0]
    readonly property color activeTextColor: Stylesheet.colors[type === 0 ? "black" : "white"]

    font: Stylesheet.fonts.label
    text: type === 0 ? 'I' : 'O'
    opacity: hovered ? 1 : 0.35

    contentItem: Label {
        text: parent.text
        font: parent.font
        color: pressed ? activeTextColor : buttonColor
    }

    background: Rectangle {
        implicitWidth: 20
        implicitHeight: 20
        color: pressed ? "transparent" : buttonColor

        border {
            color: buttonColor
            width: 1
        }
    }
}
