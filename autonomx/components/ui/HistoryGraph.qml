import QtQuick 2.0

Item {
    id: historyGraph

    property real historyLatest
    property bool historyRefresher
    property variant history: []
    property int sizeMax: 128
    property int sizeValid: 0
    property int startIndex: 0
    property color strokeColor: "#fff"

    anchors.fill: parent

    onHistoryRefresherChanged: {
        var index;
        if(sizeValid == sizeMax) {
            // buffer is full

            // index to write to is the previous start index
            index = startIndex;
            // write new value
            history[index] = historyLatest;
            // increment start index
            startIndex = (startIndex + 1) % sizeMax;
        } else {
            // buffer is not full yet

            // index to write to is at start index offset by valid size
            index = (startIndex + sizeValid) % sizeMax;
            // write new value
            history[index] = historyLatest;
            // increment valid size
            sizeValid++;
        }
    }

    Canvas {
        // add recent value

        id: graphCanvas
        anchors.fill: parent

        opacity: 0.5

        onAvailableChanged: if(available) drawGraph()

        function drawGraph() {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, graphCanvas.width, graphCanvas.height);
            ctx.strokeStyle = strokeColor;

            var lX = graphCanvas.width / (sizeMax - 1);

            ctx.beginPath();
            for (var i = 0; i < sizeValid - 1; i++) {
                var indexStart = (startIndex + sizeValid - 1 - i + sizeMax) % sizeMax;
                var indexEnd = (startIndex + sizeValid - i + sizeMax) % sizeMax;
                ctx.moveTo(graphCanvas.width - (i + 1) * lX, (1.0 - history[indexStart]) * graphCanvas.height);
                ctx.lineTo(graphCanvas.width - i * lX, (1.0 - history[indexEnd]) * graphCanvas.height)
            }
            ctx.closePath();
            ctx.stroke();

            requestAnimationFrame(drawGraph);
        }
    }
}
