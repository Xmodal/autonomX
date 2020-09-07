import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../ui"

ColumnLayout {
    id: field

    // general props
    property string labelText: "Label"
    property string propName

    property real fieldWidth: 150
    property real fieldHeight: 40
    // state props
    property bool showLabel: true
    property bool showFrame: true
    property bool fieldHovered: false
    property bool fieldFocused: false
    property bool backdropHovered: false
    property real frameMaskWidth: 0
    // flag props
    property string flagName: ""
    property bool flagValue: flagName !== "" && generatorModel.at(window.activeGeneratorIndex) ? generatorModel.at(window.activeGeneratorIndex)[flagName] : false
    property bool activated: flagName !== "" ? flag.checked : true
    // field prop
    property alias fieldContent: fieldContentLoader.sourceComponent

    // signals
    signal valueChanged(variant newValue)
    signal flagChanged(bool newFlag)

    onValueChanged: if (generatorModel.at(window.activeGeneratorIndex)) generatorModel.at(window.activeGeneratorIndex)[propName] = newValue
    onFlagChanged: if (generatorModel.at(window.activeGeneratorIndex)) generatorModel.at(activeGeneratorIndex)[flagName] = newFlag

    // layout
    implicitWidth: fieldWidth
    spacing: 5

    // top label
    Label {
        id: fieldLabel
        enabled: showLabel
        opacity: showLabel

        text: labelText
        font: Stylesheet.fonts.label
    }

    Item {
        Layout.fillWidth: true
        Layout.preferredWidth: fieldWidth
        Layout.maximumWidth: fieldWidth
        Layout.fillHeight: parent.Layout.fillHeight
        Layout.preferredHeight: fieldHeight

        FieldFrame {
            id: fieldFrame
            enabled: activated
            visible: showFrame
            isHovered: fieldHovered
            isFocused: fieldFocused
            flagEnabled: flagName !== ""
            maskWidth: field.frameMaskWidth

            // necessary to properly update flag border corner
            onBackdropHoveredChanged: field.backdropHovered = fieldFrame.backdropHovered
        }

        RowLayout {
            spacing: 0
            anchors.fill: parent

            // flag
            Item {
                visible: flagName !== ""
                Layout.preferredWidth: 30
                Layout.fillHeight: true

                CheckBox {
                    id: flag
                    checked: flagValue
                    onCheckedChanged: flagChanged(checked)
                }

                Image {
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    source: "qrc:/assets/images/check-frame.svg"
                    opacity: backdropHovered || fieldHovered || fieldFocused ? 1 : 0.3

                    Behavior on opacity {
                        NumberAnimation {
                            duration: 150
                            easing.type: Easing.Linear
                        }
                    }
                }
            }

            // field
            Loader {
                id: fieldContentLoader
                Layout.fillWidth: true
                Layout.fillHeight: true

                enabled: field.activated
                opacity: field.activated ? 1 : 0.25

                // animations
                Behavior on opacity {
                    NumberAnimation {
                        property: "opacity"
                        duration: 250
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }
    }
}
