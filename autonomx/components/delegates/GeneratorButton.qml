import QtQuick 2.9
import QtQuick.Controls 2.3

import "qrc:/stylesheet"

Button {
    id: generatorButton

    // props
    property color backgroundColor
    property string iconSource
    property bool collapsed: false

    font: Stylesheet.fonts.label
    leftPadding: 15

    contentItem: Text {
        text: generatorButton.text
        font: generatorButton.font
        color: Stylesheet.colors.white
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    indicator: Image {
        source: iconSource
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.verticalCenter: parent.verticalCenter
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        color: parent.backgroundColor
        opacity: hovered && !pressed ? 1 : 0.5
    }
}
