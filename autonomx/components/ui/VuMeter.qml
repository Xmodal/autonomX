import QtQuick 2.11

import "qrc:/stylesheet"

Item {
    property real intensity
    property color barColor: Stylesheet.colors.generator
    property int size: 6

    width: size; height: parent.height
    anchors.leftMargin: size

    // background
    Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors.white
        opacity: 0.05
    }

    // color bar
    Rectangle {
        anchors.fill: parent
        color: barColor
        antialiasing: true

        transform: Scale {
            origin.y: parent.height
            yScale: intensity
        }
    }
}
