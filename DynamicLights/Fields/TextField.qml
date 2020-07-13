import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "../Style"

Field {
    id: textField

    property string placeholder: ""
    property string defaultText: "Text Field"

    // for number fields only
    // TODO: NumberField with increment/decrement arrows
    property bool validateInt: false
    property bool unsigned: false

    IntValidator { id: validator; bottom: unsigned ? 0 : -2147483647 }

    TextField {
        id: fieldInput

        enabled: !deactivated

        // alignment
        leftPadding: 0
        Layout.fillWidth: true
        Layout.preferredHeight: 40

        // text
        text: activeFocus ? defaultText : metrics.elidedText
        placeholderText: placeholder

        // text metrics (used to elide text)
        TextMetrics {
            id: metrics
            font: fieldInput.font
            text: textField.defaultText
            elide: Text.ElideRight
            elideWidth: fieldInput.width
        }

        // background
        background: FieldFrame {
            enabled: !deactivated
            isHovered: fieldInput.hovered
            isFocused: fieldInput.activeFocus
        }

        // TODO: add validator
        // eg. no empty values allowed flag as widget property
        validator: validateInt ? validator : null

        // interactivity
        selectByMouse: true

        // signal hooks
        onEditingFinished: textField.valueChanged(text)
    }
}
