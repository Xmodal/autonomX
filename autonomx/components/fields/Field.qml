import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../ui"

ColumnLayout {
    id: field

    // general props
    property string labelText: "Label"
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
    property bool enableFlag: false
    property bool flagValue
    property bool activated: enableFlag ? flag.checked : true
    // field prop
    property Component fieldContent

    // signals
    signal valueChanged(variant newValue)
    signal flagChanged(bool newFlag)

    // layout
    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    Layout.preferredWidth: fieldWidth
    spacing: 5


    // top label
    Label {
        id: fieldLabel
        visible: showLabel

        text: labelText
        font: Stylesheet.fonts.label
    }

    Item {
        Layout.maximumWidth: fieldWidth
        Layout.preferredWidth: fieldWidth
        Layout.maximumHeight: fieldHeight
        Layout.preferredHeight: fieldHeight
        Layout.fillWidth: true
        Layout.fillHeight: true

        FieldFrame {
            id: fieldFrame
            enabled: activated
            visible: showFrame
            isHovered: fieldHovered
            isFocused: fieldFocused
            flagEnabled: enableFlag
            maskWidth: field.frameMaskWidth

            // necessary to properly update flag border corner
            onBackdropHoveredChanged: field.backdropHovered = fieldFrame.backdropHovered
        }

        RowLayout {
            spacing: 0
            anchors.fill: parent

            // flag
            Item {
                visible: enableFlag
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
                sourceComponent: fieldContent
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
