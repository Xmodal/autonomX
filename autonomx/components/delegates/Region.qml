import QtQuick 2.9
import QtQuick.Controls 2.3

import "qrc:/stylesheet"

Rectangle {
    property bool selected: type === mainContent.currRegion.type && index === mainContent.currRegion.index
    property bool dragActive: dragArea.drag.active
    property int type                       // 0 = input; 1 = output
    property real intensity: 0.0            // r => [0.0, 1.0]

    function snapToGrid() {
        colX = Math.min(Math.max(Math.round(x / mainContent.ppc), 0), 20 - colW);
        colY = Math.min(Math.max(Math.round(y / mainContent.ppc), 0), 20 - colH);
        // TODO: find better way to reevaluate property binding for coords
        x = regions.width * colX / 20.0;
        y = regions.height * colY / 20.0;

        // reevaluate matrix mask
        matrix.getRect();
    }

    antialiasing: true
    width: regions.width * colW / 20.0
    height: regions.height * colH / 20.0
    x: regions.width * colX / 20.0
    y: regions.height * colY / 20.0

    opacity: regions.rectSelected && !selected && !dragArea.containsMouse && mainContent.currRegion.type !== type ? 0.4 : 1

    color: "transparent"
    border {
        color: Stylesheet.colors[type == 0 ? "inputs" : "outputs"][index]
        width: 1
    }

    // fill rectangle
    Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors[type == 0 ? "inputs" : "outputs"][index]
        opacity: dragArea.containsMouse && !selected ? 0.5 : intensity / 2
    }

    // drag configuration
    onDragActiveChanged: {
        if (dragActive) {
            Drag.start();
            mainContent.switchSelectedRegion(type, index);
        } else {
            Drag.drop();
            snapToGrid();
        }
    }

    // index label
    Rectangle {
        width: mainContent.ppc; height: mainContent.ppc
        anchors.left: parent.right
        anchors.top: parent.top
        color: type == 0 ? Stylesheet.colors.inputs[index] : Stylesheet.colors.outputs[index]

        opacity: dragArea.containsMouse || selected ? 1 : 0

        Label {
            text: index + 1
            anchors.centerIn: parent
            font {
                weight: Font.DemiBold
                pixelSize: 13
            }
            color: Stylesheet.colors[type === 0 ? "white" : "darkGrey"]
        }
    }

    // drag area
    MouseArea {
        id: dragArea

        anchors.fill: parent
        onClicked: mainContent.switchSelectedRegion(selected ? -1 : type, selected ? -1 : index)
        hoverEnabled: true
        drag.target: parent
        cursorShape: Qt.SizeAllCursor
    }
}
