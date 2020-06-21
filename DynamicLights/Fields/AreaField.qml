import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Style"

Field {
    id: areaField

    property string placeholder: "Area Field"
    property string defaultText: "Area Field"

    TextArea {
        id: textArea

        // root settings
        text: defaultText
        placeholderText: placeholder
        wrapMode: TextEdit.WordWrap

        // alignments
        width: fieldWidth
        Layout.fillHeight: true
        leftPadding: 0
        rightPadding: 0
        topPadding: Stylesheet.field.padding
        bottomPadding: Stylesheet.field.padding

        // background
        background: FieldFrame {
            frameWidth: fieldWidth
            isHovered: textArea.hovered
            isFocused: textArea.activeFocus
        }

        // font & color
        font {
            family: Stylesheet.fonts.main
            pixelSize: 14
        }
        color: Stylesheet.colors.white

        // mouse interaction
        selectByMouse: true
    }
}
