import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../ui"

Field {
    id: checkField

    // props
    property bool on: false
    property color controlColor: Stylesheet.colors.white

    showLabel: false
    showFrame: false
    fieldWidth: 20
    fieldHeight: 20

    Layout.alignment: Qt.AlignBottom
    Layout.bottomMargin: 10
    spacing: 0

    onValueChanged: on = newValue

    fieldContent: CheckBox {
        checked: on
        onCheckedChanged: checkField.valueChanged(checked)
        controlColor: checkField.controlColor
    }
}
