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
            // TODO: adjust zoom inc/dec value by delta intensity
            if (wheel.angleDelta.y > 0)
                zoom += 0.1;
            else
                zoom -= 0.1;
        }
    }
}
