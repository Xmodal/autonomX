import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Style"

Field {
    id: sliderField

    property real minVal: 0.0
    property real maxVal: 1.0
    property real currVal: 0.5
    property real step: 0.0

    Item {
        width: parent.width
        height: 40

        // background
        FieldFrame {
            anchors.fill: parent
            isHovered: fieldSlider.hovered
            isFocused: fieldSlider.pressed
        }

        // main slider area
        RowLayout {
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
                onValueChanged: currVal = value;
            }

            // slider value indicator
            Text {
                id: sliderValue

                // alignment
                Layout.leftMargin: 0
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.preferredWidth: 40

                // text
                text: {
                    if (currVal < 10) return currVal.toFixed(2)
                    if (currVal >= 10 && currVal < 100) return currVal.toFixed(1)
                    if (currVal >= 100) return currVal
                }
                horizontalAlignment: Text.AlignRight

                // font and color
                font {
                    family: Stylesheet.fonts.main
                    weight: Font.Bold
                    pixelSize: 16
                }
                color: Stylesheet.colors.white
            }
        }
    }
}
