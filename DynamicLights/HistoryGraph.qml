import QtQuick 2.0

Item {
    id: historyGraph

    property real newValue
    property variant history: []
    property int sizeMax: 128
    property int sizeValid: 0
    property int startIndex: 0

    onNewValueChanged: graphCanvas.requestPaint()

    property color strokeColor: "#fff"

    anchors.fill: parent

    Canvas {
        // add recent value

        id: graphCanvas
        anchors.fill: parent

        opacity: 0.5

        onPaint: {
            if(sizeValid == sizeMax) {
                // buffer is full

                // index to write to is the previous start index
                let index = startIndex;
                // write new value
                history[index] = newValue;
                // increment start index
                startIndex = (startIndex + 1) % sizeMax;
            } else {
                // buffer is not full yet

                // index to write to is at start index offset by valid size
                let index = (startIndex + sizeValid) % sizeMax;
                // write new value
                history[index] = newValue;
                // increment valid size
                sizeValid++;
            }

            var ctx = getContext("2d");
            ctx.clearRect(0, 0, graphCanvas.width, graphCanvas.height);
            ctx.strokeStyle = strokeColor;

            var lX = graphCanvas.width / (sizeMax - 1);

            ctx.beginPath();
            for (var i = 0; i < sizeValid - 1; i++) {
                var indexStart = (startIndex + sizeValid - 1 - i + sizeMax) % sizeMax;
                var indexEnd = (startIndex + sizeValid - i + sizeMax) % sizeMax;
                ctx.moveTo(graphCanvas.width - i * lX, (1.0 - history[indexStart]) * graphCanvas.height);
                ctx.lineTo(graphCanvas.width - (i + 1) * lX, (1.0 - history[indexEnd]) * graphCanvas.height)
            }
            ctx.closePath();
            ctx.stroke();
        }
    }
}
