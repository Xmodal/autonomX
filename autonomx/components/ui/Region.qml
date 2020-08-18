import QtQuick 2.9
import QtQuick.Controls 2.3

import "qrc:/stylesheet"

Rectangle {
    id: region

    // props
    property bool selected: type === latticeView.currRegion.type && index === latticeView.currRegion.index
    property bool dragActive: dragArea.drag.active
    property bool inEdit: false
    property int latticeWidth
    property int latticeHeight
    property int type                       // 0 = input; 1 = output
    property int resizeHitbox: 5            // zone padding
    property int area
    property int ppc

    antialiasing: true

    // position
    width: regions.width * rect.width / regions.latticeWidth
    height: regions.height * rect.height / regions.latticeHeight
    x: regions.width * rect.x / regions.latticeWidth
    y: regions.height * rect.y / regions.latticeHeight
    z: selected ? 10 : 10 / area;

    // break x/y property bindings
    Component.onCompleted: {
        x = x;
        y = y;
        width = width;
        height = height;

        area = rect.width * rect.height;
        area = area;
    }

    // matrix.setMask() update slots
    onXChanged: updateBounds()
    onYChanged: updateBounds()
    onWidthChanged: updateBounds()
    onHeightChanged: updateBounds()

    // reposition region if out of bounds
    onLatticeWidthChanged: snapToGrid("drag")
    onLatticeHeightChanged: snapToGrid("drag")

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
        var newRect = Qt.rect(
                    Math.max(Math.round(x / ppc), 0),
                    Math.max(Math.round(y / ppc), 0),
                    Math.max(Math.round((width - 1 + offsetW) / ppc), 0),
                    Math.max(Math.round((height - 1 + offsetH) / ppc), 0)
                    );

        // clamp depending on event type
        if (evtType === "resize") {
            if (newRect.x + newRect.width > regions.latticeWidth)  newRect.width = regions.latticeWidth - newRect.x;
            if (newRect.y + newRect.height > regions.latticeHeight) newRect.height = regions.latticeHeight - newRect.y;
        } else if (evtType === "drag") {
            if (newRect.x + newRect.width > regions.latticeWidth)  newRect.x = regions.latticeWidth - newRect.width;
            if (newRect.y + newRect.height > regions.latticeHeight) newRect.y = regions.latticeHeight - newRect.height;
        }

        // update model here
        rect = newRect;
        area = rect.width * rect.height;

        // then reevaluate pixel coordinates by animating them
        snapX.to = regions.width * rect.x / regions.latticeWidth;
        snapY.to = regions.height * rect.y / regions.latticeHeight;
        snapWidth.to = regions.width * rect.width / regions.latticeWidth;
        snapHeight.to = regions.height * rect.height / regions.latticeHeight;
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
        matrix.setMask(Qt.rect(
            x / ppc,
            y / ppc,
            (width - 1) / ppc,
            (height - 1) / ppc
        ));
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
    opacity: regions.rectSelected && !selected && !dragArea.containsMouse ? (latticeView.currRegion.type !== type ? 0.4 : 0.6) : 1

    // fill rectangle
    Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors[type == 0 ? "inputs" : "outputs"][index]
        opacity: dragArea.containsMouse && (!selected || inEdit) ? 0.5 : intensity / 2
    }

    // drag configuration
    onDragActiveChanged: {
        if (dragActive) {
            Drag.start();
            latticeView.switchSelectedRegion(type, index);
            inEdit = true;
            changeCursor(Qt.SizeAllCursor);
        } else {
            Drag.drop();
            snapToGrid("drag");
            inEdit = false;
            changeCursor();
        }
    }

    // index label
    Rectangle {
        width: ppc; height: ppc
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
        onClicked: latticeView.switchSelectedRegion(selected ? -1 : type, selected ? -1 : index)
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
                inEdit = pressed;

                // on mouse release
                if (!pressed) snapToGrid("resize");
            }

            onMouseXChanged: {
                if (!pressed || !"lr".includes(modelData.charAt(1))) return;

                // set new width
                var newWidth = region.width;
                newWidth -= modelData.charAt(1) === "l" ? mouseX : -mouseX;

                // clamp width
                if (newWidth < ppc) {
                    newWidth = ppc;
                    if (mouseX > 0 && region.width !== ppc) region.x += region.width - ppc;
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
                if (newHeight < ppc) {
                    newHeight = ppc;
                    if (mouseY > 0 && region.height !== ppc) region.y += region.height - ppc;
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
