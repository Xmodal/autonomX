import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "../Style"

Field {
    id: textField

    property string placeholder: "Placeholder"
    property string defaultText: ""

    fieldContent: TextField {
        id: fieldInput

        // alignment
        Layout.fillWidth: true
        leftPadding: Stylesheet.field.padding_LR
        rightPadding: Stylesheet.field.padding_LR
        topPadding: Stylesheet.field.padding_TB
        bottomPadding: Stylesheet.field.padding_TB

        // text
        text: defaultText
        placeholderText: placeholder

        // font + color
        font {
            family: Stylesheet.fonts.main
            pointSize: 14
        }
        color: Stylesheet.colors.white
        // placeholderTextColor: Stylesheet.setAlpha(Stylesheet.colors.white, 0.4)

        // background
        background: FieldFrame {
            // frameWidth: fieldWidth
            isHovered: hovered
            isFocused: fieldInput.activeFocus
        }

        // interactivity
        selectByMouse: true
    }
}
