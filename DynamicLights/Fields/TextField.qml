import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "../Style"

Field {
    id: textField

    property string placeholder: ""
    property string defaultText: "Text Field"

    TextField {
        id: fieldInput

        // alignment
        Layout.fillWidth: true
        leftPadding: 0
        height: 40

        // text
        text: defaultText

        placeholderText: placeholder

        // font + color
        font {
            family: Stylesheet.fonts.main
            pixelSize: 18
        }
        color: Stylesheet.colors.white

        // background
        background: FieldFrame {
            frameWidth: fieldWidth
            isHovered: fieldInput.hovered
            isFocused: fieldInput.activeFocus
        }

        // TODO: add validator
        // eg. no empty values allowed flag as widget property

        // interactivity
        selectByMouse: true
    }
}
