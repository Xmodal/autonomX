import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../ui"

ColumnLayout {
    id: field

    // general props
    property string labelText: "Label"
    property var target: generatorModel.at(activeGeneratorIndex)
    property string propName
    property real fieldWidth: 160
    property color fieldBg: Stylesheet.colors.black
    // state props
    property bool showLabel: true
    property bool showFrame: true
    property bool fieldHovered: false
    property bool fieldFocused: false
    property bool backdropHovered: false
    property real frameMaskWidth: 0
    // flag props
    property string flagName: ""
    property bool flagValue: flagName !== "" && target ? target[flagName] : false
    property bool activated: flagName !== "" ? flag.checked : true
    // field prop
    property alias fieldContent: fieldContentLoader.sourceComponent
    // signal error
    // TODO: implement this properly in the next sprint
    property bool hasError: false

    // signals
    signal valueChanged(variant newValue)
    signal flagChanged(bool newFlag)
    signal errorReceived(string title, string errorText)

    // signal handlers
    property var valueChangedHandler: function(newValue) {
        if (target) target[propName] = newValue
    }
    property var flagChangedHandler: function(newFlag) {
        if (target) taregt[flagName] = newFlag
    }

    // signal assigners
    // this is so that handlers are always unique
    // and can be overridden
    onValueChanged: valueChangedHandler(newValue)
    onFlagChanged: flagChangedHandler(newFlag)

    // layout
    Layout.preferredWidth: fieldWidth
    spacing: 5

    // reset prop name + flag name to prevent void linking when changing generator type
    function reset() {
        flagName = "";
        propName = null;
    }

    // top label
    Label {
        id: fieldLabel
        enabled: showLabel
        opacity: showLabel

        text: labelText
        font: Stylesheet.fonts.label
    }

    Item {
        // background
        Rectangle {
            anchors.fill: parent
            color: fieldBg
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
        Layout.preferredHeight: fieldContent.height

        // popup
//        ErrorPopup {
//            id: errorPopup
//            enabled: hasError
//            opacity: hasError

//            anchors.left: parent.right
//            anchors.bottom: parent.bottom
//            anchors.leftMargin: 5

//            Behavior on opacity {
//                NumberAnimation { duration: 300 }
//            }
//        }

        // content
        ColumnLayout {
            id: fieldContent

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

        // border
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            opacity: fieldFocused ? 0.6 : 0
            border {
                width: 1
                color: Stylesheet.colors.white
            }
        }
    }
}
