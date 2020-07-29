import QtQuick 2.9

import "qrc:/stylesheet"

Rectangle {
    property bool selected: index === mainContent.currRegionIndex
    property bool dragActive: dragArea.drag.active

    function snapToGrid() {
        x = Math.min(Math.max(Math.round(x / mainContent.ppc), 0), 20 - colW) * mainContent.ppc;
        y = Math.min(Math.max(Math.round(y / mainContent.ppc), 0), 20 - colH) * mainContent.ppc;
    }

    antialiasing: true
    width: regions.width * colW / 20.0
    height: regions.height * colH / 20.0
    x: regions.width * colX / 20.0
    y: regions.height * colY / 20.0

    opacity: regions.rectSelected && !selected ? 0.2 : 1

    color: "transparent"
    border {
        color: type == 0 ? Stylesheet.colors.inputs[0] : Stylesheet.colors.outputs[0]
        width: 1
    }

    // drag configuration
    Drag.hotSpot {
        x: 40
        y: 40
    }
    onDragActiveChanged: {
        if (dragActive) {
            Drag.start();
        } else {
            Drag.drop();
            snapToGrid();
        }
    }

    // drag area
    MouseArea {
        id: dragArea

        anchors.fill: parent
        onClicked: mainContent.switchSelectedRegion(index)
        drag.target: parent
        cursorShape: Qt.SizeAllCursor
    }
}
