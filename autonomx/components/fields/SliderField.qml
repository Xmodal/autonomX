import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: sliderField

    property real minVal: 0.0
    property real maxVal: 1.0
    property real currVal: propName ? generatorModel.at(window.activeGeneratorIndex)[propName] : 0.5
    property real exponent: 1.0
    property int precision: 5
    property real step: 0.0
    property real updateLag: 0.0

    function parseValue(v) {
        return Number(v.toFixed(precision))
    }

    // main slider area
    fieldContent: Item {
        id: sliderContainer

        property real innerMaskWidth: Math.max((sliderValue.contentWidth + 14) / width, 0.3)
        onInnerMaskWidthChanged: frameMaskWidth = innerMaskWidth

        // value update lag timer
        Timer {
            id: sliderLagTimer
            interval: updateLag; repeat: false; running: false
            triggeredOnStart: false

            onTriggered: sliderField.valueChanged(slider.value)
        }

        ColumnLayout {
            spacing: 3
            anchors.fill: parent

            // slider
            Slider {
                id: slider

                // alignment
                Layout.fillWidth: true
                Layout.preferredHeight: 10
                Layout.topMargin: 10
                padding: 0

                onHoveredChanged: fieldHovered = hovered
                onPressedChanged: fieldFocused = pressed

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
                        color: Stylesheet.colors.generator
                    }
                }

                // no handle
                handle: Rectangle {
                    width: 0; height: parent.height
                    x: slider.visualPosition * slider.availableWidth; y: 0
                }

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
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter

                // text
                text: parseValue(slider.value)
                horizontalAlignment: Text.AlignHCenter
                validator: DoubleValidator { bottom: minVal; top: maxVal }
                selectByMouse: true

                // field frame
                onHoveredChanged: fieldHovered = hovered
                onActiveFocusChanged: fieldFocused = activeFocus

                // background (none)
                background: Item {}

                // offset to the left (not sure why I need this, but I Do)
                transform: Translate { x: -2 }

                // signals
                onEditingFinished: {
                    var newValue = displayText;

                    if (newValue === "") newValue = (minVal + maxVal) / 2;
                    else if (newValue > maxVal) newValue = maxVal;
                    else if (newValue < minVal) newValue = minVal;

                    sliderField.valueChanged(newValue);

                    focus = false;
                }
            }

            // states & transitions
            // current state (apparently I have to set it like this for it to work properly instead of using "when" in each state)
    //        state: slider.pressed ? "pressed" : (slider.hovered ? "hovered" : "")
    //        states: [
    //            State {
    //                name: "hovered";
    //                PropertyChanges { target: trueHandle; handleColor: Stylesheet.colors.white }
    //            },

    //            State {
    //                name: "pressed";
    //                PropertyChanges { target: trueHandle; handleColor: Stylesheet.colors.generator }
    //            }
    //        ]
    //        transitions: [
    //            Transition {
    //                from: ""; to: "hovered"
    //                ColorAnimation { target: trueHandle; properties: "handleColor"; duration: 250; easing.type: Easing.InOutQuad }
    //            },

    //            Transition {
    //                from: "hovered"; to: ""
    //                ColorAnimation { target: trueHandle; properties: "handleColor"; duration: 250; easing.type: Easing.InOutQuad }
    //            }
    //        ]
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/
