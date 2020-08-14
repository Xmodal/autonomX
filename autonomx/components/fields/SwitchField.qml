import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: switchField

    property bool on: false
    property color controlColor: Stylesheet.colors.generator

    spacing: 5

    fieldContent: Switch {
        id: switchObj

        enabled: !deactivated

        checked: switchField.on
        onToggled: switchField.valueChanged(checked)

        indicator: Rectangle {
            id: indicatorBg
            implicitWidth: 55
            implicitHeight: 30
            radius: implicitHeight / 2
            color: Stylesheet.colors.black

            Rectangle {
                id: indicator
                width: 20; height: 20
                x: 5; y: 5
                radius: width / 2
                color: "#4C4C4C"
            }
        }

        states: State {
            name: "checked"; when: switchObj.checked
            PropertyChanges {
                target: indicator
                x: indicatorBg.implicitWidth - indicator.width - 5
                color: controlColor
            }
        }

        transitions: Transition {
            NumberAnimation {
                target: indicator
                property: "x"
                duration: 350
                easing.type: Easing.OutQuad
            }

            ColorAnimation {
                target: indicator
                duration: 350
                easing.type: Easing.OutQuad
            }
        }
    }
}
