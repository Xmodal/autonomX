import QtQuick 2.9
import QtQuick.Controls 2.9

import "qrc:/stylesheet"

Button {
    property int type: 0                    // input = 0; output = 1

    readonly property color buttonColor: Stylesheet.colors[type === 0 ? "inputs" : "outputs"][0]
    readonly property color activeTextColor: Stylesheet.colors[type === 0 ? "black" : "white"]

    font: Stylesheet.fonts.label
    text: type === 0 ? 'I' : 'O'
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
