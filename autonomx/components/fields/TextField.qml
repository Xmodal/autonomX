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

        // alignment
        leftPadding: 0

        // text
        //text: activeFocus ? defaultText : metrics.elidedText
        text: defaultText
        placeholderText: placeholder

        // field frame events
        onHoveredChanged: fieldHovered = hovered
        onActiveFocusChanged: fieldFocused = activeFocus

        // text metrics (used to elide text)
//        TextMetrics {
//            id: metrics
//            font: fieldInput.font
//            text: textField.defaultText
//            elide: Text.ElideRight
//            elideWidth: fieldInput.width
//        }

        background: Item {}

        // interactivity
        selectByMouse: true

        // signal hooks
        onEditingFinished: {
            valueChanged(text);
            focus = false;
        }
    }
}
