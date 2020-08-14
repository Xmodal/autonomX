import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: checkField

    // props
    property bool on: false
    property color controlColor: Stylesheet.colors.white

    showLabel: false
    fieldWidth: 20

    Layout.preferredHeight: 20
    Layout.alignment: Qt.AlignBottom
    Layout.bottomMargin: 10
    spacing: 0

    onValueChanged: on = newValue



    fieldContent: CheckBox {
        id: control
        implicitWidth: 20
        implicitHeight: 20
        checked: on
        onCheckedChanged: checkField.valueChanged(checked)

        property real scaleValue: checked ? (hovered ? 1.2 : 1) : 0

        background: Rectangle {
            id: controlFrame
            width: 20; height: 20;
            color: "transparent"
            border {
                width: 1
                color: Stylesheet.colors.white
            }
            opacity: control.hovered || control.checked ? 1 : 0.5

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
            antialiasing: true

            transform: Scale {
                xScale: scaleValue
                yScale: scaleValue
                origin {
                    x: 5
                    y: 5
                }
            }
        }

        Behavior on scaleValue {
            NumberAnimation { duration: 350; easing.type: Easing.OutCubic }
        }
    }
}
