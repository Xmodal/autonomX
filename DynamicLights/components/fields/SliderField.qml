import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: sliderField

    property real minVal: 0.0
    property real maxVal: 1.0
    property real currVal: 0.5
    property real exponent: 1.0
    property int precision: 5
    property real step: 0.0
    property real updateLag: 0.0

    function parseValue(v) {
        return Number(v.toFixed(precision))
    }

    // value update lag timer
    Timer {
        id: sliderLagTimer
        interval: updateLag; repeat: false; running: false
        triggeredOnStart: false

        onTriggered: sliderField.valueChanged(slider.value)
    }

    // main slider area
    ColumnLayout {
        id: sliderContainer
        Layout.preferredHeight: 40
        spacing: 3

        // slider
        Slider {
            id: slider

            enabled: !deactivated

            // alignment
            Layout.fillWidth: true
            Layout.preferredHeight: 10 + trueHandle.height
            padding: 0

            // slider params
            from: minVal
            to: maxVal
            value: currVal
            stepSize: step

            // value bar
            background: Rectangle {
                width: parent.width
                height: 10
                color: "#4D4D4D"

                Rectangle {
                    width: parent.width * slider.visualPosition
                    anchors.left: parent.left
                    height: parent.height
                    // TODO: change 0 to generator index
                    color: Stylesheet.colors.generators[0]
                }
            }

            // no handle
            handle: Rectangle {
                width: 0; height: parent.height
                x: slider.visualPosition * slider.availableWidth; y: 0
            }

            // actual handle
            Canvas {
                id: trueHandle

                property color handleColor: Stylesheet.setAlpha(Stylesheet.colors.white, 0.25)
                onHandleColorChanged: requestPaint()

                x: (slider.visualPosition * slider.availableWidth) - (width / 2)
                y: 10
                height: 8
                width: 10

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.clearRect(0, 0, width, height);

                    ctx.fillStyle = handleColor;

                    ctx.beginPath();
                    ctx.moveTo(0, 8);
                    ctx.lineTo(5, 0);
                    ctx.lineTo(10, 8);
                    ctx.closePath();
                    ctx.fill();
                }
            }

            // signals
            onValueChanged: {
                if (updateLag > 0) sliderLagTimer.restart();
                else sliderField.valueChanged(value)
            }
        }

        TextField {
            id: sliderValue

            enabled: !deactivated

            // alignment
            padding: 0
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter

            // text
            text: parseValue(slider.value)
            horizontalAlignment: Text.AlignHCenter
            validator: DoubleValidator { bottom: minVal; top: maxVal }
            selectByMouse: true

            background: Rectangle { opacity: 0 }

            // signals
            onEditingFinished: {
                if (displayText === "") currVal = (minVal + maxVal) / 2;
                else if (displayText > maxVal) currVal = maxVal;
                else if (displayText < minVal) currVal = minVal;
                else currVal = displayText;
            }
        }

        // states & transitions
        // current state (apparently I have to set it like this for it to work properly instead of using "when" in each state)
        state: slider.pressed ? "pressed" : (slider.hovered ? "hovered" : "")
        states: [
            State {
                name: "hovered";
                PropertyChanges { target: trueHandle; handleColor: Stylesheet.colors.white }
            },

            State {
                name: "pressed";
                PropertyChanges { target: trueHandle; handleColor: Stylesheet.colors.generators[0] }
            }
        ]
        transitions: [
            Transition {
                from: ""; to: "hovered"
                ColorAnimation { target: trueHandle; properties: "handleColor"; duration: 250; easing.type: Easing.InOutQuad }
            },

            Transition {
                from: "hovered"; to: ""
                ColorAnimation { target: trueHandle; properties: "handleColor"; duration: 250; easing.type: Easing.InOutQuad }
            }
        ]
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/
