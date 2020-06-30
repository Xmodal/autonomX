import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "../Style"

Field {
    id: textField

    property string placeholder: ""
    property string defaultText: "Text Field"
    property bool validateInt: false

    IntValidator { id: validator }

    TextField {
        id: fieldInput

        // alignment
        Layout.fillWidth: true
        leftPadding: 0
        height: 40

        // text
        text: activeFocus ? defaultText : metrics.elidedText
        placeholderText: placeholder

        // font + color
        font {
            pixelSize: 18
        }

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
