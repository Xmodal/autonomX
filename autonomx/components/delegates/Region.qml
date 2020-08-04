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
    z: selected ? 3 : 0;

    // break x/y property bindings
    Component.onCompleted: {
        x = x;
        y = y;
        width = width;
        height = height;
    }
    onXChanged: updateBounds()
    onYChanged: updateBounds()
    onWidthChanged: updateBounds()
    onHeightChanged: updateBounds()

    // snap to grid on drop / resize
    function snapToGrid() {
        // clamp w/h when left corner/border dragged in the negatives
        width += Math.min(x, 0);
        height += Math.min(y, 0);

        // update model here
        colW = Math.min(Math.max(Math.round((width - 1) / mainContent.ppc), 0), mainContent.cols - colX);
        colH = Math.min(Math.max(Math.round((height - 1) / mainContent.ppc), 0), mainContent.rows - colY);
        colX = Math.min(Math.max(Math.round(x / mainContent.ppc), 0), mainContent.cols - colW);
        colY = Math.min(Math.max(Math.round(y / mainContent.ppc), 0), mainContent.rows - colH);

        // then reevaluate region coordinates
        x = regions.width * colX / mainContent.cols;
        y = regions.height * colY / mainContent.rows;
        width = regions.width * colW / mainContent.cols;
        height = regions.height * colH / mainContent.rows;

        // THEN reevaluate the matrix mask rectangle
        matrix.getRect();
    }

    // update region bounds
    // TODO: segment this into different functions
    // to limit calculation rate to what's necessary to calculate
    // (also find a way to only call matrix.getRect() once per mouse interaction)
    function updateBounds() {
        var newColX = Math.round(x / mainContent.ppc);
        var newColY = Math.round(y / mainContent.ppc);
        var newColW = Math.round((width - 1) / mainContent.ppc);
        var newColH = Math.round((height - 1) / mainContent.ppc);

        matrix.getRect(newColX, newColY, newColW, newColH);
    }

    // force cursor on event
    // this modifies properties on a MouseArea component over in LatticeView
    function changeCursor(cursorShape) {
        matrixMouseBg.z = cursorShape ? 5 : 0;
        matrixMouseBg.cursorShape = cursorShape || Qt.ArrowCursor;
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
            changeCursor(Qt.SizeAllCursor);
        } else {
            Drag.drop();
            snapToGrid();
            changeCursor();
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
        cursorShape: selected ? Qt.SizeAllCursor : Qt.PointingHandCursor
    }

    // resize areas
    Repeater {
        // hash map
        model: ["ttV", "rrH", "bbV", "llH", "tlF", "trB", "brF", "blB"]

        MouseArea {
            visible: selected

            anchors {
                top: modelData.charAt(0) === "t" ? parent.top : undefined
                bottom: modelData.charAt(0) === "b" ? parent.bottom : undefined
                left: modelData.charAt(1) === "l" ? parent.left : undefined
                right: modelData.charAt(1) === "r" ? parent.right : undefined
            }

            width:  modelData.charAt(0) === modelData.charAt(1) && "tb".includes(modelData.charAt(0)) ? parent.width : resizeHitbox
            height: modelData.charAt(0) === modelData.charAt(1) && "lr".includes(modelData.charAt(0)) ? parent.height : resizeHitbox

            cursorShape: {
                switch (modelData.charAt(2)) {
                    case "V": return Qt.SizeVerCursor;
                    case "H": return Qt.SizeHorCursor;
                    case "B": return Qt.SizeBDiagCursor;
                    case "F": return Qt.SizeFDiagCursor;
                }
            }

            onPressedChanged: {
                // unified cursor change on press/release
                changeCursor(pressed ? cursorShape : null);

                // on mouse release
                if (!pressed) snapToGrid();
            }

            onMouseXChanged: {
                if (!pressed) return;

                // update width
                if ("lr".includes(modelData.charAt(1))) {
                    region.width -= modelData.charAt(1) === "l" ? mouseX : -mouseX;
                }

                // clamp width
                if (region.width < mainContent.ppc) region.width = mainContent.ppc;
                // update X
                else if (modelData.charAt(1) === "l") region.x += mouseX;
            }

            onMouseYChanged: {
                if (!pressed) return;

                // update height
                if ("tb".includes(modelData.charAt(0))) {
                    region.height -= modelData.charAt(0) === "t" ? mouseY : -mouseY;
                }

                // clamp height
                if (region.height < mainContent.ppc) region.height = mainContent.ppc;
                // update Y
                else if (modelData.charAt(0) === "t") region.y += mouseY;
            }
        }
    }
}
