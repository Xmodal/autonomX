import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "../Style"

Field {
    id: selectField

    property int index: 0
    property variant options: ["A", "B", "C"]

    ComboBox {
        id: comboBox

        // root settings
        currentIndex: index
        model: options
        font {
            family: Stylesheet.fonts.main
            pixelSize: 18
        }

        // background
        background: FieldFrame {
            frameWidth: fieldWidth
            isHovered: comboBox.hovered
        }

        // options delegate
        delegate: ItemDelegate {
            id: itemDlgt
            width: comboBox.width
            padding: 0

            // item contents
            contentItem: Text {
                text: modelData
                color: Stylesheet.colors.white
                opacity: hovered ? 1 : 0.5
                font.family: comboBox.font.family
                font.pixelSize: 14
                padding: Stylesheet.field.padding
            }

            // item background
            background: Rectangle {
                color: Stylesheet.colors.white
                opacity: hovered ? 0.1 : 0
                border.width: 0
                width: parent.width
                height: parent.height
            }
        }

        // selected item contents
        contentItem: Label {
            text: comboBox.displayText
            verticalAlignment: Text.AlignVCenter
            height: 40

            color: Stylesheet.colors.white
        }

        // popup
        popup: Popup {
            id: cbPopup
            y: comboBox.height
            width: comboBox.width
            height: contentHeight
            padding: 0

            contentItem: ListView {
                id: popupList
                implicitHeight: contentHeight
                model: comboBox.popup.visible ? comboBox.delegateModel : null
            }

            background: Rectangle {
                color: Stylesheet.colors.black
                border.width: 0
            }
        }

        // down caret indicator
        indicator: Image {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            id: indicator
            source: "qrc:/assets/images/down-caret.svg"

            // auto rotate
            transform: Rotation {
                angle: comboBox.popup.visible ? 180 : 0
                origin.x: indicator.width / 2
                origin.y: indicator.height / 2
            }
        }
    }
}
