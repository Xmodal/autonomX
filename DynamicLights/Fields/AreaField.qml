import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Style"

Field {
    id: areaField

    property string placeholder: "Area Field"
    property string defaultText: "Area Field"

    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        FieldFrame {
            isHovered: textArea.hovered
            isFocused: textArea.activeFocus
        }

        Flickable {
            id: flickable
            anchors.fill: parent
            maximumFlickVelocity: 350

            TextArea.flickable: TextArea {
                id: textArea

                // root settings
                text: defaultText
                placeholderText: placeholder
                wrapMode: TextArea.Wrap

                // alignments
                width: fieldWidth
                leftPadding: 0
                rightPadding: 0
                topPadding: Stylesheet.field.padding
                bottomPadding: Stylesheet.field.padding

                // background
                background: Rectangle { opacity: 0 }

                // font & color
                font {
                    family: Stylesheet.fonts.main
                    pixelSize: 14
                }
                color: Stylesheet.colors.white

                // mouse interaction
                selectByMouse: true
            }

            ScrollBar.vertical: ScrollBar {}
        }
    }
}
