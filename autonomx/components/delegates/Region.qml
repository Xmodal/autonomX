import QtQuick 2.9

import "qrc:/stylesheet"

Rectangle {
    property bool selected: index === mainContent.currRegionIndex

    antialiasing: true
    width: regions.width * colW / 20.0
    height: regions.height * colH / 20.0
    x: regions.width * colX / 20.0
    y: regions.height * colY / 20.0

    opacity: regions.rectSelected && !selected ? 0.2 : 1

    color: "transparent"
    border {
        color: type == 0 ? Stylesheet.colors.input : Stylesheet.colors.output
        width: 1
    }

    // drag area
    MouseArea {
        anchors.fill: parent
        onClicked: mainContent.switchSelectedRegion(index)
        cursorShape: Qt.SizeAllCursor
    }
}
