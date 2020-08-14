import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: textField

    property string placeholder: ""
    property string defaultText: "Text Field"

    fieldContent: TextField {
        id: fieldInput

        enabled: !deactivated

        // alignment
        leftPadding: 0

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

        // interactivity
        selectByMouse: true

        // signal hooks
        onEditingFinished: textField.valueChanged(text)
    }
}
