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

    function parseValue(v) {
        return v.toFixed(2 - Math.floor(Math.log10(v < 1 ? 1 : v)))
    }

    Item {
        Layout.fillWidth: true
        Layout.preferredHeight: 40

        // background
        FieldFrame {
            isHovered: fieldSlider.hovered || sliderValue.hovered
            isFocused: fieldSlider.pressed || sliderValue.activeFocus
        }

        // main slider area
        RowLayout {
            id: sliderContainer
            anchors.fill: parent

            // slider
            Slider {
                id: fieldSlider

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
                        width: parent.width * fieldSlider.visualPosition
                        anchors.left: parent.left
                        height: parent.height
                        color: Stylesheet.colors.outputs[0]
                    }
                }

                // no handle
                handle: Rectangle {}

                // signals
                onValueChanged: sliderField.valueChanged(value)
            }

            TextField {
                id: sliderValue

                // alignment
                padding: 0
                Layout.leftMargin: 0
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.preferredWidth: 50

                // text
                text: parseValue(currVal)
                horizontalAlignment: Text.AlignRight
                validator: RegExpValidator { regExp: /\d*.*\d*/ }
                selectByMouse: true

                // font and color
                font {
                    family: Stylesheet.fonts.main
                    weight: Font.Bold
                    pixelSize: 16
                }
                background: Rectangle { opacity: 0 }
                color: Stylesheet.colors.white

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
