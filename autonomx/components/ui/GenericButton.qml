import QtQuick 2.9
import QtQuick.Controls 2.2

import "qrc:/stylesheet"

Button {
    id: genericButton

    property color activeColor: Stylesheet.colors.generator

    topPadding: 4
    bottomPadding: 4
    leftPadding: 15
    rightPadding: 15

    contentItem: Label {
        font: Stylesheet.fonts.label
        opacity: hovered ? 1 : 0.75
        text: genericButton.text
    }

    background: Rectangle {
        anchors.fill: parent
        color: pressed ? activeColor : (hovered ? Stylesheet.colors.black : Stylesheet.colors.darkGrey)
    }

    CursorShaper {
        id: mouseArea
    }
}
