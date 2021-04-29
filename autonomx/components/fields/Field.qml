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

    property real fieldWidth: 160
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
        Rectangle {
            anchors.fill: parent
            color: Stylesheet.colors.black
            opacity: fieldFocused || fieldHovered ? 1 : 0.5

            Behavior on opacity {
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.Linear
                }
            }
        }

        // layout
        Layout.fillWidth: true
        Layout.preferredWidth: fieldWidth
        Layout.maximumWidth: fieldWidth
        Layout.fillHeight: parent.Layout.fillHeight
        Layout.preferredHeight: childrenRect.height

        // content
        ColumnLayout {
            spacing: 0
            anchors {
                left: parent.left
                right: parent.right
                leftMargin: 10
                rightMargin: 10
                topMargin: 7
                bottomMargin: 7
            }

            // flag
            CheckBox {
                id: flag
                visible: flagName !== ""

                Layout.preferredWidth: 20
                Layout.topMargin: -25
                Layout.bottomMargin: 5
                Layout.rightMargin: -10
                Layout.alignment: Qt.AlignRight

                checked: flagValue
                onCheckedChanged: flagChanged(checked)
            }

            // field
            Loader {
                id: fieldContentLoader
                Layout.fillWidth: true
                Layout.fillHeight: true

                enabled: field.activated
                opacity: field.activated ? 1 : 0.4

                // animations
                Behavior on opacity {
                    NumberAnimation {
                        duration: 250
                        easing.type: Easing.InOutQuad
                    }
                }
            }

            // mouse blocker
            MouseArea {
                Layout.topMargin: -fieldContentLoader.implicitHeight
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.leftMargin: -10
                Layout.rightMargin: -10
                hoverEnabled: true

                visible: !field.activated
                cursorShape: Qt.ForbiddenCursor
            }
        }
    }
}
