import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Style"

Field {
    id: sliderField

    property real minVal: 0.0
    property real maxVal: 1.0
    property real currVal: 0.5
    property real exponent: 1.0
    property real step: 0.0
    property real updateLag: 0.0

    function parseValue(v) {
        return v.toFixed(2 - Math.floor(Math.log10(v < 1 ? 1 : v)))
    }

    Item {
        Layout.fillWidth: true
        Layout.preferredHeight: 40

        // value update lag timer
        Timer {
            id: sliderLagTimer
            interval: updateLag; repeat: false; running: false
            triggeredOnStart: false

            onTriggered: sliderField.valueChanged(slider.value)
        }

        // background
        FieldFrame {
            isHovered: slider.hovered || sliderValue.hovered
            isFocused: slider.pressed || sliderValue.activeFocus
        }

        // main slider area
        RowLayout {
            id: sliderContainer
            anchors.fill: parent

            // slider
            Slider {
                id: slider

                // alignment
                Layout.fillWidth: true

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
                        color: Stylesheet.colors.outputs[0]
                    }
                }

                // no handle
                handle: Rectangle {}

                // signals
                onValueChanged: {
                    if (updateLag > 0) sliderLagTimer.restart();
                    else sliderField.valueChanged(value)
                }
            }

            TextField {
                id: sliderValue

                // alignment
                padding: 0
                Layout.leftMargin: 0
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.preferredWidth: 50

                // text
                text: parseValue(slider.value)
                horizontalAlignment: Text.AlignRight
                validator: RegExpValidator { regExp: /\d*.*\d*/ }
                selectByMouse: true

                // font and color
                font {
                    weight: Font.Bold
                    pixelSize: 16
                }
                background: Rectangle { opacity: 0 }

                // signals
                onEditingFinished: {
                    if (displayText === "") currVal = (minVal + maxVal) / 2;
                    else if (displayText > maxVal) currVal = maxVal;
                    else if (displayText < minVal) currVal = minVal;
                    else currVal = displayText;
                }
            }
        }
    }
}
