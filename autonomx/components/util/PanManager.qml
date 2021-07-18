import QtQuick 2.12

Item {
    id: panManager
    x: 0
    y: 0
    width: parent.width
    height: parent.height
    enabled: !(generatorIndex < 0)

    Drag.active: panDragArea.drag.active

    property vector2d dragValue: Qt.vector2d(x, y)
    property vector2d prevDragValue: Qt.vector2d(0, 0);
    property bool allowPan: false               // used so that the pan doesn't snap back to (0, 0) on button release
    property bool isDraggingRegion: false       // changed by Region dragging and/or resizing

    // master props
    property real zoomExp: 0
    property real zoom: zoomCoarse
    property real zoomCoarse: Math.pow(2, zoomExp) * 100
    property real panX: 0
    property real panY: 0
    property vector2d panCoarse: Qt.vector2d(panX, panY)
    property real easedPanX: panX
    property real easedPanY: panY
    property vector2d pan: Qt.vector2d(easedPanX, easedPanY)

    // minmax range values for zoomExp, not zoomCoarse!
    readonly property real maxZoom: 2.5
    readonly property real minZoom: -2.5

    // shortcut functions
    function zoomIn(offset = 10) {
        zoomExp += offset;
        if (zoomExp >= maxZoom) zoomExp = maxZoom;
    }
    function zoomOut(offset = 10) {
        zoomExp -= offset;
        if (zoomExp <= minZoom) zoomExp = minZoom;
    }
    function resetView() {
        zoomExp = panX = panY = 0;
    }

    // animations
    Behavior on easedPanX {
        enabled: !allowPan      // do not ease on middle mouse panning
        SpringAnimation {
            spring: 5
            epsilon: 0.25
            damping: 1
            mass: 1
        }
    }
    Behavior on easedPanY {
        enabled: !allowPan      // do not ease on middle mouse panning
        SpringAnimation {
            spring: 5
            epsilon: 0.25
            damping: 1
            mass: 1
        }
    }
    Behavior on zoom {
        SpringAnimation {
            spring: 5
            epsilon: 0.25
            damping: 1
            mass: 1
        }
    }

    onDragValueChanged: {
        if (!allowPan || isDraggingRegion) return;

        // calculate difference
        var newDrag = prevDragValue.minus(dragValue);

        // add to pan value
        panX += newDrag.x;
        panY += newDrag.y;
        // reset previous drag
        prevDragValue = dragValue;
    }

    MouseArea {
        id: panDragArea
        anchors.fill: parent
        acceptedButtons: Qt.MiddleButton        // because we middle only 8)
        preventStealing: true

        // drag props
        drag.target: parent
        drag.threshold: 0
        drag.smoothed: false

        // pan handling
        onPressed: {
            overrideCursor(Qt.ClosedHandCursor);
            parent.allowPan = true;
        }
        onReleased: {
            restoreCursor();
            parent.allowPan = false;

            // reinitialize drag values
            parent.x = 0
            parent.y = 0
            parent.prevDragValue = Qt.vector2d(0, 0);
        }

        // zoom handling
        onWheel: {
            if (isDraggingRegion) return;

            // get coords of mouse where the wheel evt was triggered
            // map to [-size/2, size/2] range
            let wheelDiff = Qt.vector2d(wheel.x, wheel.y).minus(Qt.vector2d(width / 2, height / 2));
            let offset = 0.1;

            // save previous zoom to calculate offset
            // because the above prop is relative to the exponent, not the power
            const prevCoarse = zoomCoarse;

            // increment or decrement zoom
            // TODO: adjust zoom inc/dec value by delta intensity (not necessary actually)
            if (wheel.angleDelta.y > 0)
                zoomIn(offset);
            else
                zoomOut(offset);

            // zoom according to mouse position
            // (Epic Formula TM in the Line Below)
            wheelDiff = wheelDiff.plus(panCoarse).times((zoomCoarse - prevCoarse) / zoomCoarse);

            // add to global pan vector
            panX += wheelDiff.x;
            panY += wheelDiff.y;
        }
    }
}
