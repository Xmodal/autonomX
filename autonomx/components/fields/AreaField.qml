import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: areaField

    property string placeholder: ""
    property string defaultText: propName && generatorModel.at(window.activeGeneratorIndex) ? generatorModel.at(window.activeGeneratorIndex)[propName] : "Text Field"

    fieldContent: Flickable {
        id: flickable
        anchors.fill: parent
        maximumFlickVelocity: 350
        implicitHeight: 100
        interactive: fieldInput.contentHeight > height

        TextArea.flickable: TextArea {
            id: fieldInput

            // alignment
            leftPadding: 0
            topPadding: 7
            bottomPadding: 7

            // text
            font: Stylesheet.fonts.text
            text: defaultText
            placeholderText: placeholder
            wrapMode: TextArea.Wrap

            // field frame events
            onHoveredChanged: fieldHovered = hovered
            onActiveFocusChanged: {
                window.editingTextField = activeFocus
                fieldFocused = activeFocus
            }

            background: Item {}

            // interactivity
            selectByMouse: true

            // signal hooks
            onEditingFinished: {
                areaField.valueChanged(text);
                focus = false;
            }
        }

        ScrollBar.vertical: ScrollBar {
            interactive: false
        }
    }
}
