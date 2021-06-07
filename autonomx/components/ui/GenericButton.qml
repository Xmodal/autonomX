import QtQuick 2.9
import QtQuick.Controls 2.2

import "qrc:/stylesheet"

Button {
    id: genericButton

    property color activeColor: Stylesheet.colors.generator
    property color neutralColor: Stylesheet.colors.darkGrey
    property color hoverColor: Stylesheet.colors.black

    property real paddingSide: 15

    height: 24

    leftPadding: paddingSide
    rightPadding: paddingSide

    contentItem: Label {
        font: Stylesheet.fonts.label
        opacity: hovered ? 1 : 0.75
        text: genericButton.text
        verticalAlignment: Qt.AlignVCenter
    }

    background: Rectangle {
        implicitHeight: parent.height
        color: pressed ? activeColor : (hovered ? hoverColor : neutralColor)
    }

    CursorShaper {
        id: mouseArea
    }
}
