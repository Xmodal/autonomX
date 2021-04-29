import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../ui"

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
        property bool altPressed: window.altPressed
        onAltPressedChanged: updateDragCursor()

        // generalized drag cursor update process
        function updateDragCursor(restore = false) {
            if (altPressed) {
                if (cursorShaper.containsMouse)
                    return overrideCursor(Qt.SizeHorCursor)

                if (restore) restoreCursor()
            }
        }

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

            // treat empty values
            if (newValue === "") newValue = (minVal + maxVal) / 2;
            // clamp to min/max
            else if (newValue > maxVal) newValue = maxVal;
            else if (newValue < minVal) newValue = minVal;

            // signal to facade
            sliderField.valueChanged(newValue);

            // blur
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

        MouseArea {
            id: cursorShaper
            anchors.fill: parent
            visible: sliderField.activated && altPressed     // override active only on alt press+hold
            hoverEnabled: true

            // slider value is being changed here
            property real dragX: slideDragger.x
            property real prevDragX: 0
            onDragXChanged: {
                // prevents value bounce when mouse is released from drag state
                if (!window.allowSlideDrag) {
                    prevDragX = 0;
                    return;
                }

                if (containsMouse) {
                    var newDragX = prevDragX - dragX;

                    // calculate value displacement
                    var pixelUnit = (slider.to - slider.from) / slider.width;
                    var displaceValue = newDragX * pixelUnit;
                    if (shiftPressed) displaceValue *= 0.2;         // fine tuning
                    // effectively update slider value
                    slider.value -= displaceValue;

                    // set previous value
                    prevDragX = dragX;
                }
            }

            // this is where we detect when to
            onHoveredChanged: {
                updateDragCursor(true)

                if (containsMouse) window.allowSlideDrag = true
                else window.allowSlideDrag = false
            }
        }
    }
}
