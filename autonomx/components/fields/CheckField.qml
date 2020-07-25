import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: checkField

    showLabel: false
    fieldWidth: 20
    Layout.preferredHeight: 20
    Layout.alignment: Qt.AlignBottom
    Layout.bottomMargin: 10
    spacing: 0

    property bool on: false
    onValueChanged: on = newValue
    property color controlColor: Stylesheet.colors.white

    CheckBox {
        id: control
        implicitWidth: 20
        implicitHeight: 20
        checked: on
        onCheckedChanged: checkField.valueChanged(checked)

        background: Rectangle {
            id: controlFrame
            width: 20; height: 20;
            color: "transparent"
            border {
                width: 1
                color: Stylesheet.colors.white
            }
            opacity: 0.5

            Behavior on opacity {
                NumberAnimation {
                    property: "opacity"
                    duration: 150
                    easing.type: Easing.Linear
                }
            }
        }

        indicator: Rectangle {
            x: 5; y: 5;
            width: 10; height: 10
            color: controlColor
            visible: control.checked
        }

        states: State {
            name: "hovered"; when: control.hovered || control.checked
            PropertyChanges { target: controlFrame; opacity: 1 }
        }
    }
}
