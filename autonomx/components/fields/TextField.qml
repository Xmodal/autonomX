import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: textField

    property string placeholder: ""
    property string defaultText: propName && generatorModel.at(window.activeGeneratorIndex) ? generatorModel.at(window.activeGeneratorIndex)[propName] : "Text Field"

    fieldContent: TextField {
        id: fieldInput

        // alignment
        leftPadding: 0

        // text
        font: Stylesheet.fonts.fieldText
        text: activeFocus ? defaultText : metrics.elidedText
        placeholderText: placeholder

        // field frame events
        onHoveredChanged: fieldHovered = hovered
        onActiveFocusChanged: fieldFocused = activeFocus

        // text metrics (used to elide text)
        TextMetrics {
            id: metrics
            font: fieldInput.font
            text: defaultText
            elide: Text.ElideRight
            elideWidth: fieldWidth - 10
        }

        background: Item {}

        // interactivity
        selectByMouse: true

        // signal hooks
        onEditingFinished: {
            textField.valueChanged(text);
            focus = false;
        }
    }
}
