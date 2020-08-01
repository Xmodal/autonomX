import QtQuick 2.9
import QtQuick.Controls 2.3

import "qrc:/stylesheet"

Rectangle {
    id: region

    // props
    property bool selected: type === mainContent.currRegion.type && index === mainContent.currRegion.index
    property bool dragActive: dragArea.drag.active
    property int type                       // 0 = input; 1 = output
    property real intensity: 0.0            // r => [0.0, 1.0]
    property int resizeHitbox: 5            // zone padding

    antialiasing: true

    // position
    width: regions.width * colW / mainContent.cols
    height: regions.height * colH / mainContent.rows
    x: regions.width * colX / mainContent.cols
    y: regions.height * colY / mainContent.rows

    // break x/y property bindings
    Component.onCompleted: {
        x = x;
        y = y;
    }

    // manage auto mask follow
    onXChanged: {
        var newColX = Math.min(Math.max(Math.round(x / mainContent.ppc), 0), mainContent.cols - colW);

        if (newColX !== colX) {
            colX = newColX;
            matrix.getRect();
        }
    }
    onYChanged: {
        var newColY = Math.min(Math.max(Math.round(y / mainContent.ppc), 0), mainContent.rows - colH);

        if (newColY !== colY) {
            colY = newColY;
            matrix.getRect();
        }
    }

    // snap to grid on drop
    function snapToGrid() {
        x = regions.width * colX / mainContent.cols;
        y = regions.height * colY / mainContent.rows;
    }

    // external region boundary
    color: "transparent"
    border {
        color: Stylesheet.colors[type == 0 ? "inputs" : "outputs"][index]
        width: 1
    }
    // border opacity
    opacity: regions.rectSelected && !selected && !dragArea.containsMouse && mainContent.currRegion.type !== type ? 0.4 : 1

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

    // drag/select area
    MouseArea {
        id: dragArea

        anchors.fill: parent
        onClicked: mainContent.switchSelectedRegion(selected ? -1 : type, selected ? -1 : index)
        propagateComposedEvents: true
        hoverEnabled: true
        drag.target: region
        cursorShape: Qt.SizeAllCursor
    }

    // resize areas
    Repeater {
        // hash map
        model: ["ttV", "rrH", "bbV", "llH", "tlF", "trB", "brF", "blB"]

        MouseArea {
            anchors {
                top: modelData.charAt(0) === "t" ? parent.top : undefined
                bottom: modelData.charAt(0) === "b" ? parent.bottom : undefined
                left: modelData.charAt(1) === "l" ? parent.left : undefined
                right: modelData.charAt(1) === "r" ? parent.right : undefined
            }

            width: resizeHitbox + (modelData.charAt(0) === modelData.charAt(1) && "tb".includes(modelData.charAt(0)) ? parent.width : 0)
            height: resizeHitbox + (modelData.charAt(0) === modelData.charAt(1) && "lr".includes(modelData.charAt(0)) ? parent.height : 0)

            cursorShape: {
                switch (modelData.charAt(2)) {
                    case "V": return Qt.SizeVerCursor;
                    case "H": return Qt.SizeHorCursor;
                    case "B": return Qt.SizeBDiagCursor;
                    case "F": return Qt.SizeFDiagCursor;
                }
            }
        }
    }
}
