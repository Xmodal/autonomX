import QtQuick 2.12
import QtQuick.Controls 2.12

import ca.hexagram.xmodal.autonomx 1.0

import "qrc:/stylesheet"

Control {
    id: region

    // props
    property bool dragActive: dragArea.drag.active
    property bool inEdit: false
    // only changed by type=-1 region
    property bool allowUpdatingBounds: true

    property int latticeWidth: regions.latticeWidth
    property int latticeHeight: regions.latticeHeight
    property real ppc: latticeView.ppc
    property int type: 0                       // 0 = input; 1 = output
    property int resizeHitbox: 5            // zone padding
    property int area: rect.width * rect.height

    property color currColor: type < 0 ? Stylesheet.colors.white : Stylesheet.colors[type == 0 ? "inputs" : "outputs"][(index < 0 ? 0 : index) % Stylesheet.colors.variations]

    // tab navigation enabled when selected
    focus: type === latticeView.currRegion.type && index === latticeView.currRegion.index
    onFocusChanged: {
        if (focus) {
            forceActiveFocus()
            switchSelectedRegion(type, index)
        }
    }
    // constrain tab navigation to region typeset when selected
    focusPolicy: latticeView.currRegion.type === type ? Qt.TabFocus : Qt.NoFocus
    KeyNavigation.backtab: index === 0 ? this : null
    KeyNavigation.tab: index === generatorModel.at(generatorIndex)[type ? "outputCount" : "inputCount"] - 1 ? this : null

    // antialiasing
    antialiasing: true

    // position
    width: regions.width * rect.width / latticeWidth
    height: regions.height * rect.height / latticeHeight
    x: regions.width * rect.x / latticeWidth
    y: regions.height * rect.y / latticeHeight
    z: focus ? 10 : 10 / area;

    // break x/y property bindings
    Component.onCompleted: {
        // assign size change auto-drag once region is created
        // (for some reason, having them initialized pre-completion
        // causes uninvoked snapping when going from a generator to another)
        const sizeDrag = () => region.snapToGrid("drag");

        const connectWidth = () => {
            region.latticeWidthChanged.disconnect(connectWidth);
            region.latticeWidthChanged.connect(sizeDrag);
        }

        const connectHeight = () => {
            region.latticeHeightChanged.disconnect(connectHeight);
            region.latticeHeightChanged.connect(sizeDrag);
        }

        region.latticeWidthChanged.connect(connectWidth);
        region.latticeHeightChanged.connect(connectHeight);
    }

    // matrix.setMask() update slots
    onXChanged: updateBounds()
    onYChanged: updateBounds()
    onWidthChanged: updateBounds()
    onHeightChanged: updateBounds()

    // animation triggers
    NumberAnimation on x { id: snapX; running: false; duration: 250; easing.type: Easing.OutCirc; onFinished: rebind("x"); }
    NumberAnimation on y { id: snapY; running: false; duration: 250; easing.type: Easing.OutCirc; onFinished: rebind("y"); }
    NumberAnimation on width { id: snapWidth; running: false; duration: 250; easing.type: Easing.OutCirc; onFinished: rebind("width"); }
    NumberAnimation on height { id: snapHeight; running: false; duration: 250; easing.type: Easing.OutCirc; onFinished: rebind("height"); }

    // rect coords rebinding process
    // called when associated NumberAnimation is done
    function rebind(prop) {
        switch (prop) {
            case "x":
                x = Qt.binding(function() { return regions.width * rect.x / latticeWidth })
                break
            case "y":
                y = Qt.binding(function() { return regions.height * rect.y / latticeHeight; })
                break
            case "width":
                width = Qt.binding(function() { return regions.width * rect.width / latticeWidth })
                break
            case "height":
                height = Qt.binding(function() { return regions.height * rect.height / latticeHeight })
                break
        }
    }

    // snap to grid on drop / resize
    function snapToGrid(evtType) {
        console.log("asdf")

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

        if (newRect.width > regions.latticeWidth) newRect.width = regions.latticeWidth;
        if (newRect.height > regions.latticeHeight) newRect.height = regions.latticeHeight;

        // clamp depending on event type
        if (evtType === "resize") {
            if (newRect.x + newRect.width > regions.latticeWidth)  newRect.width = regions.latticeWidth - newRect.x;
            if (newRect.y + newRect.height > regions.latticeHeight) newRect.height = regions.latticeHeight - newRect.y;
        } else if (evtType === "drag") {
            if (newRect.x + newRect.width > regions.latticeWidth)  newRect.x = regions.latticeWidth - newRect.width;
            if (newRect.y + newRect.height > regions.latticeHeight) newRect.y = regions.latticeHeight - newRect.height;
        }

        // unbind
        x = x;
        y = y;
        width = width;
        height = height;

        // update model here
        rect = newRect;

        // then reevaluate pixel coordinates by animating them
        snapX.to = regions.width * rect.x / regions.latticeWidth;
        snapY.to = regions.height * rect.y / regions.latticeHeight;
        snapWidth.to = regions.width * rect.width / regions.latticeWidth;
        snapHeight.to = regions.height * rect.height / regions.latticeHeight;
        snapX.restart();
        snapY.restart();
        snapWidth.restart();
        snapHeight.restart();

        // rebind individually on animation end
    }

    // update region bounds
    // TODO: segment this into different functions
    // to limit calculation rate to what's necessary to calculate
    // (also find a way to only call matrix.setMask() once per mouse interaction)
    function updateBounds() {
        matrix.setMask(Qt.rect(x, y, width - 1, height - 1));
    }

    // force cursor on event
    // this modifies properties on a MouseArea component over in LatticeView
    function changeCursor(cursorShape) {
        matrixMouseBg.z = cursorShape ? 5 : 0;
        matrixMouseBg.cursorShape = cursorShape || Qt.ArrowCursor;
    }


    // region keyboard management
    Keys.onPressed: {
        let offset;

        // region navigation
        switch (event.key) {
            case Qt.Key_Left:
                offset = Qt.point(-1, 0);
                break;
            case Qt.Key_Right:
                offset = Qt.point(1, 0);
                break;
            case Qt.Key_Up:
                offset = Qt.point(0, -1);
                break;
            case Qt.Key_Down:
                offset = Qt.point(0, 1);
                break;

            // delete
            case Qt.Key_Delete:
                return deleteCurrentRegion();

            default:
                return;
        }

        this[shiftPressed ? (altPressed ? "collapse" : "extend") : "nudge"](offset.x, offset.y);
    }

    // nudge region
    function nudge(xOff, yOff) {
        x += xOff * ppc;
        y += yOff * ppc;

        snapToGrid("drag");
    }
    // extend edges
    function extend(xOff, yOff) {
        x += xOff < 0 ? xOff * ppc : 0;
        y += yOff < 0 ? yOff * ppc : 0;
        width += xOff !== 0 ? Math.abs(xOff * ppc) : 0;
        height += yOff !== 0 ? Math.abs(yOff * ppc) : 0;
        width = Math.max(width, ppc);
        height = Math.max(height, ppc);

        snapToGrid("drag");
    }
    // collapse edges
    function collapse(xOff, yOff) {
        x += xOff < 0 ? 0 : xOff * ppc;
        y += yOff < 0 ? 0 : yOff * ppc;
        width -= xOff !== 0 ? Math.abs(xOff * ppc) : 0;
        height -= yOff !== 0 ? Math.abs(yOff * ppc) : 0;
        width = Math.max(width, ppc);
        height = Math.max(height, ppc);

        snapToGrid("drag");
    }


    // external region boundary
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border {
            color: currColor
            width: regions.rectSelected && region.focus ? 2 : 1
        }
        // border opacity
        opacity: regions.rectSelected && !(type < 0) && !region.focus && !dragArea.containsMouse ? (latticeView.currRegion.type !== type ? 0.3 : 0.5) : 1
    }

    // fill rectangle
    Rectangle {
        anchors.fill: parent
        color: currColor
        opacity: dragArea.containsMouse && (!region.focus || inEdit) ? 0.5 : (1.0 - Math.pow(1.0 - intensity, 3)) * 0.8
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
        color: currColor

        opacity: dragArea.containsMouse && !(type < 0) || region.focus ? 1 : 0

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
        onClicked: latticeView.switchSelectedRegion(region.focus ? -1 : type, region.focus ? -1 : index)
        propagateComposedEvents: true
        hoverEnabled: true
        drag.target: region
        drag.threshold: 0
        cursorShape: region.focus ? Qt.SizeAllCursor : Qt.PointingHandCursor
    }

    // resize areas
    Repeater {
        // hash map
        model: ["ttV", "rrH", "bbV", "llH", "tlF", "trB", "brF", "blB"]

        MouseArea {
            visible: region.focus

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
