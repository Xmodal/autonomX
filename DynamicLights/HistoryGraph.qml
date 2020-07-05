import QtQuick 2.0

Item {
    id: historyGraph

    property int startIndex
    property int sizeValid
    property int sizeMax
    property variant points

    onStartIndexChanged: graphCanvas.requestPaint()

    property color strokeColor: "#fff"

    anchors.fill: parent

    Canvas {
        id: graphCanvas
        anchors.fill: parent

        opacity: 0.5

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, graphCanvas.width, graphCanvas.height);
            ctx.strokeStyle = strokeColor;

            var lX = graphCanvas.width / (sizeMax - 1);

            ctx.beginPath();
            for (var i = 0; i < sizeMax - 1; i++) {
                ctx.moveTo(i * lX, points[(startIndex + i) % sizeMax] * graphCanvas.height);
                ctx.lineTo((i + 1) * lX, points[(startIndex + i + 1) % sizeMax] * graphCanvas.height)
            }
            ctx.closePath();
            ctx.stroke();
        }
    }
}
