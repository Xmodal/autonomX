import QtQuick 2.9
import QtQuick.Controls 2.2

import "qrc:/stylesheet"

Button {
    id: genericButton

    property color activeColor: Stylesheet.colors.generator
    property color neutralColor: Stylesheet.colors.darkGrey
    property color hoverColor: Stylesheet.colors.black
    property color activeTextColor: Stylesheet.colors.white

    property real paddingSide: 15

    height: 24

    leftPadding: paddingSide
    rightPadding: paddingSide

    contentItem: Label {
        color: pressed ? activeTextColor : Stylesheet.colors.white
        font: Stylesheet.fonts.label

        opacity: hovered || pressed ? 1 : 0.6
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
