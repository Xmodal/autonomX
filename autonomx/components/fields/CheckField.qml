// probably deprecated

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../ui"

Field {
    id: checkField

    // props
    property bool on: propName ? generatorModel.at(window.activeGeneratorIndex)[propName] : false
    property color controlColor: Stylesheet.colors.white

    showLabel: false
    showFrame: false
    fieldWidth: 20

    fieldContent: Item {
        CheckBox {
            anchors.verticalCenter: parent.verticalCenter
            checked: on
            onCheckedChanged: checkField.valueChanged(checked)
            controlColor: checkField.controlColor
        }
    }
}
