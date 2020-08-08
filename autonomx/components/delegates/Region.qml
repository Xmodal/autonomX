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
    property int area

    antialiasing: true

    // position
    width: regions.width * colW / mainContent.cols
    height: regions.height * colH / mainContent.rows
    x: regions.width * colX / mainContent.cols
    y: regions.height * colY / mainContent.rows
    z: selected ? 10 : 10 / area;

    // break x/y property bindings
    Component.onCompleted: {
        x = x;
        y = y;
        width = width;
        height = height;

        area = colW * colH;
        area = area;
    }

    // matrix.setMask() update slots
    onXChanged: updateBounds()
    onYChanged: updateBounds()
    onWidthChanged: updateBounds()
    onHeightChanged: updateBounds()

    // animation triggers
    NumberAnimation on x { id: snapX; running: false; duration: 250; easing.type: Easing.OutCirc; }
    NumberAnimation on y { id: snapY; running: false; duration: 250; easing.type: Easing.OutCirc; }
    NumberAnimation on width { id: snapWidth; running: false; duration: 250; easing.type: Easing.OutCirc; }
    NumberAnimation on height { id: snapHeight; running: false; duration: 250; easing.type: Easing.OutCirc; }

    // snap to grid on drop / resize
    function snapToGrid(evtType) {
        // clamp w/h when left corner/border dragged in the negatives
        var offsetW = 0, offsetH = 0;
        if (evtType === "resize") {
            offsetW = Math.min(x, 0);
            offsetH = Math.min(y, 0);
        }

        // calculate unsigned cell coordinates
        var newColX = Math.max(Math.round(x / mainContent.ppc), 0);
        var newColY = Math.max(Math.round(y / mainContent.ppc), 0);
        var newColW = Math.max(Math.round((width - 1 + offsetW) / mainContent.ppc), 0);
        var newColH = Math.max(Math.round((height - 1 + offsetH) / mainContent.ppc), 0);

        // clamp depending on event type
        if (evtType === "resize") {
            if (newColX + newColW > mainContent.cols) newColW = mainContent.cols - newColX;
            if (newColY + newColH > mainContent.rows) newColH = mainContent.rows - newColY;
        } else if (evtType === "drag") {
            if (newColX + newColW > mainContent.cols) newColX = mainContent.cols - newColW;
            if (newColY + newColH > mainContent.rows) newColY = mainContent.rows - newColH;
        }

        // update model here
        colX = newColX;
        colY = newColY
        colW = newColW;
        colH = newColH;
        area = colW * colH;

        // then reevaluate pixel coordinates by animating them
        snapX.to = regions.width * colX / mainContent.cols;
        snapY.to = regions.height * colY / mainContent.rows;
        snapWidth.to = regions.width * colW / mainContent.cols;
        snapHeight.to = regions.height * colH / mainContent.rows;
        snapX.restart();
        snapY.restart();
        snapWidth.restart();
        snapHeight.restart();
    }

    // update region bounds
    // TODO: segment this into different functions
    // to limit calculation rate to what's necessary to calculate
    // (also find a way to only call matrix.setMask() once per mouse interaction)
    function updateBounds() {
        matrix.setMask(
            x / mainContent.ppc,
            y / mainContent.ppc,
            (width - 1) / mainContent.ppc,
            (height - 1) / mainContent.ppc
        );
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
            snapToGrid("drag");
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
        drag.threshold: 0
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
                if (!pressed) snapToGrid("resize");
            }

            onMouseXChanged: {
                if (!pressed || !"lr".includes(modelData.charAt(1))) return;

                // set new width
                var newWidth = region.width;
                newWidth -= modelData.charAt(1) === "l" ? mouseX : -mouseX;

                // clamp width
                if (newWidth < mainContent.ppc) {
                    newWidth = mainContent.ppc;
                    if (mouseX > 0 && region.width !== mainContent.ppc) region.x += region.width - mainContent.ppc;
                }

                // update X
                else if (modelData.charAt(1) === "l") {
                    region.x += mouseX;
                }

                // update width
                region.width = newWidth;
            }

            onMouseYChanged: {
                if (!pressed || !"tb".includes(modelData.charAt(0))) return;

                // set new height
                var newHeight = region.height;
                newHeight -= modelData.charAt(0) === "t" ? mouseY : -mouseY;

                // clamp height
                if (newHeight < mainContent.ppc) {
                    newHeight = mainContent.ppc;
                    if (mouseY > 0 && region.height !== mainContent.ppc) region.y += region.height - mainContent.ppc;
                // update Y
                } else if (modelData.charAt(0) === "t") {
                    region.y += mouseY;
                }

                // update height
                region.height = newHeight;
            }
        }
    }
}
