import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: sliderField

    property real minVal: 0.0
    property real maxVal: 1.0
    property real currVal: propName && generatorModel.at(window.activeGeneratorIndex) ? generatorModel.at(window.activeGeneratorIndex)[propName] : 0.5
    property real exponent: 1.0
    property int precision: 5
    property real step: 0.0
    property real updateLag: 0.0

    function parseValue(v) {
        return Number(v.toFixed(precision))
    }

    // main slider area
    fieldContent: TextField {
        // value update lag timer
        Timer {
            id: sliderLagTimer
            interval: updateLag; repeat: false; running: false
            triggeredOnStart: false

            onTriggered: sliderField.valueChanged(slider.value)
        }

        id: sliderValue

        // alignment
        leftPadding: 0

        // text
        text: parseValue(slider.value)
        validator: DoubleValidator { bottom: minVal; top: maxVal }
        selectByMouse: true

        // field frame
        onHoveredChanged: fieldHovered = hovered
        onActiveFocusChanged: fieldFocused = activeFocus

        // background (none)
        background: Item {}

        // signals
        onEditingFinished: {
            var newValue = displayText;

            if (newValue === "") newValue = (minVal + maxVal) / 2;
            else if (newValue > maxVal) newValue = maxVal;
            else if (newValue < minVal) newValue = minVal;

            sliderField.valueChanged(newValue);

            focus = false;
        }

        // slider
        Slider {
            id: slider

            // alignment
            anchors.bottom: parent.bottom
            implicitWidth: parent.width + 20
            anchors.left: parent.left
            anchors.leftMargin: -10
            implicitHeight: 4
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
                width: parent.width * slider.visualPosition
                anchors.left: parent.left
                height: parent.height
                color: Stylesheet.colors.generator
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
    }
}
