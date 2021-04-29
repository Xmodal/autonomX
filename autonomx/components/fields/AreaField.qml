import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: areaField

    property string placeholder: "Text Area"
    //property string defaultText: propName && generatorModel.at(window.activeGeneratorIndex) ? generatorModel.at(window.activeGeneratorIndex)[propName] : ""
    property string defaultText

    fieldContent: Flickable {
        id: flickable
        anchors.fill: parent
        maximumFlickVelocity: 350
        height: 50
        interactive: textArea.contentHeight > height

        TextArea.flickable: TextArea {
            id: textArea

            // root settings
            text: defaultText
            placeholderText: placeholder
            wrapMode: TextArea.Wrap

            // field frame
            onHoveredChanged: fieldHovered = hovered
            onActiveFocusChanged: fieldFocused = activeFocus

            // alignments
            width: fieldWidth
            leftPadding: 0
            rightPadding: 0
            topPadding: Stylesheet.field.padding
            bottomPadding: Stylesheet.field.padding

            // background
            background: Item {}

            // font & color
            font.pixelSize: 14

            // mouse interaction
            selectByMouse: true

            // signal hooks
            onEditingFinished: {
                valueChanged(text);
                focus = false;
            }
        }

        ScrollBar.vertical: ScrollBar {
            interactive: false
        }
    }
}
