import QtQuick 2.11
import QtQuick.Controls 2.2

import "qrc:/stylesheet"

Rectangle {
    id: dummyRegion

    property int latticeWidth: regions.latticeWidth
    property int latticeHeight: regions.latticeHeight
    property real ppc: latticeView.ppc

    property point start
    property point end

    // computed rect values
    property rect area: {
        // coords
        let x = start.x < end.x ? start.x : end.x;
        let y = start.y < end.y ? start.y : end.y;

        // dimensions
        let w = end.x - start.x;
        let h = end.y - start.y;

        // lock bounds
        if (dragging) {
            x = Math.max(0, x);
            y = Math.max(0, y);

            w = Math.min(Math.max(-start.x, w), latticeWidth - start.x - 1);
            h = Math.min(Math.max(-start.y, h), latticeHeight - start.y - 1);
        }

        w = Math.abs(w) + 1;
        h = Math.abs(h) + 1;

        // return
        return Qt.rect(x, y, w, h);
    }

    property bool dragging: false     // code for "currently clicking"
    property bool insideBounds: area.x >= 0 && area.x < latticeWidth && area.y >= 0 && area.y < latticeHeight

    width: area.width * ppc
    height: area.height * ppc
    x: area.x * ppc
    y: area.y * ppc
    z: 1000000000

    // external region boundary
    color: "transparent"
    border {
        color: Stylesheet.colors.white
        width: 1
    }
    // border opacity
    opacity: insideBounds ? 1 : 0.4

    // fill rectangle
    Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors.white
        opacity: dragging ? 0.2 : 0
    }
}
