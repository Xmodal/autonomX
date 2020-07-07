import QtQuick 2.0

Item {
    id: historyGraph

    property int startIndex
    property int sizeValid
    property int sizeMax
    property variant points

    onPointsChanged: graphCanvas.requestPaint()

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
            for (var i = 0; i < sizeValid - 1; i++) {
                var indexStart = (startIndex + sizeValid - 1 - i + sizeMax) % sizeMax;
                var indexEnd = (startIndex + sizeValid - i + sizeMax) % sizeMax;
                ctx.moveTo(graphCanvas.width - i * lX, points[indexStart] * graphCanvas.height);
                ctx.lineTo(graphCanvas.width - (i + 1) * lX, points[indexEnd] * graphCanvas.height)
            }
            ctx.closePath();
            ctx.stroke();
        }
    }
}
