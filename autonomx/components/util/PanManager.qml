import QtQuick 2.12

Item {
    id: panDragger
    x: 0
    y: 0
    width: parent.width
    height: parent.height
    enabled: !(generatorIndex < 0)

    Drag.active: panDragArea.drag.active

    property vector2d dragValue: Qt.vector2d(x, y)
    property vector2d prevDragValue: Qt.vector2d(0, 0);
    property bool allowPan: false

    onDragValueChanged: {
        if (allowPan) {
            // calculate difference
            var newDrag = prevDragValue.minus(dragValue);

            // add to pan value
            latticeView.pan = latticeView.pan.plus(newDrag);
            // reset previous drag
            prevDragValue = dragValue;
        }
    }

    MouseArea {
        id: panDragArea
        anchors.fill: parent
        acceptedButtons: Qt.MiddleButton
        preventStealing: true

        drag.target: parent
        drag.threshold: 0
        drag.smoothed: false

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

        // zoomies!!!!!! yeeeeehaaaaawwwwww
        onWheel: {
            // get coords of mouse where the wheel evt was triggered
            // map to [-size/2, size/2] range
            let wheelDiff = Qt.vector2d(wheel.x, wheel.y).minus(Qt.vector2d(width / 2, height / 2));

            // increment or decrement zoom
            // TODO: adjust zoom inc/dec value by delta intensity
            if (wheel.angleDelta.y > 0) {
                zoomPercent += 5;
                if (zoomPercent >= 500) zoomPercent = 500;
            } else {
                zoomPercent -= 5;
                if (zoomPercent <= 10) zoomPercent = 10;
            }

            // zoom according to mouse position
            // (Epic Formula TM in the Line Below)
            wheelDiff = wheelDiff.plus(latticeView.pan).times(0.05 * (1 / zoom));
            if (wheel.angleDelta.y < 0) wheelDiff = wheelDiff.times(-1);

            // add to global pan vector
            latticeView.pan = latticeView.pan.plus(wheelDiff);
        }
    }
}
