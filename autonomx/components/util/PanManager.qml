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
    property vector2d panCoarse: Qt.vector2d(panX, panY)
    property vector2d pan: Qt.vector2d(easedPanX, easedPanY)
    property real panX: 0
    property real panY: 0
    property real easedPanX: panX
    property real easedPanY: panY
    property real zoomCoarse: 100
    property real zoom: zoomCoarse

    readonly property real maxZoom: 500
    readonly property real minZoom: 10

    // shortcut functions
    function zoomIn(offset = 10) {
        zoomCoarse += offset;
        if (zoomCoarse >= maxZoom) zoomCoarse = maxZoom;
    }
    function zoomOut(offset = 10) {
        zoomCoarse -= offset;
        if (zoomCoarse <= minZoom) zoomCoarse = minZoom;
    }

    // animations
    Behavior on easedPanX {
        enabled: !allowPan
        SpringAnimation {
            spring: 5
            epsilon: 0.25
            damping: 1
            mass: 3
        }
    }
    Behavior on easedPanY {
        enabled: !allowPan
        SpringAnimation {
            spring: 5
            epsilon: 0.25
            damping: 1
            mass: 3
        }
    }
    Behavior on zoom {
        SpringAnimation {
            spring: 5
            epsilon: 0.25
            damping: 1
            mass: 3
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
            let offset = 10;

            // increment or decrement zoom
            // TODO: adjust zoom inc/dec value by delta intensity
            if (wheel.angleDelta.y > 0)
                zoomIn(offset);
            else
                zoomOut(offset);

            // zoom according to mouse position
            // (Epic Formula TM in the Line Below)
            wheelDiff = wheelDiff.plus(panCoarse).times(offset / zoomCoarse);
            if (wheel.angleDelta.y < 0) wheelDiff = wheelDiff.times(-1);

            // add to global pan vector
            panX += wheelDiff.x;
            panY += wheelDiff.y;
        }
    }
}
