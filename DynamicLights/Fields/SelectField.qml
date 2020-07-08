import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "../Style"

Field {
    id: selectField

    property int index
    property variant options

    ComboBox {
        id: comboBox

        // root settings
        currentIndex: selectField.index
        model: options

        onCurrentIndexChanged: selectField.valueChanged(currentIndex)

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
                opacity: hovered ? 1 : 0.5
                font.family: comboBox.font.family
                font.pixelSize: 14
                color: Stylesheet.colors.white
                padding: Stylesheet.field.padding
            }

            // item background
            background: Rectangle {
                anchors.fill: parent
                opacity: hovered ? 0.1 : 0
                border.width: 0
            }
        }

        // selected item contents
        contentItem: Label {
            text: comboBox.displayText
            verticalAlignment: Text.AlignVCenter
            height: 40
        }

        // popup
        popup: Popup {
            id: cbPopup
            y: comboBox.height
            width: comboBox.width
            height: contentItem.implicitHeight < 240 ? contentItem.implicitHeight : 240
            padding: 0

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: comboBox.popup.visible ? comboBox.delegateModel : null

                ScrollIndicator.vertical: ScrollIndicator {}
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
