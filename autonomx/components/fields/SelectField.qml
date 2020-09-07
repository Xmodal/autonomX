import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: selectField

    property int index: propName && generatorModel.at(window.activeGeneratorIndex) ? generatorModel.at(window.activeGeneratorIndex)[propName] : 0
    property variant options

    fieldContent: ComboBox {
        id: comboBox

        // root settings
        currentIndex: selectField.index
        model: options

        onCurrentIndexChanged: selectField.valueChanged(currentIndex)
        onHoveredChanged: fieldHovered = hovered
        onDownChanged: fieldFocused = down

        background: Item {}

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
                model: comboBox.down ? comboBox.delegateModel : null

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
                id: indicatorRot
                origin.x: indicator.width / 2
                origin.y: indicator.height / 2
                angle: comboBox.down ? 180 : 0

                Behavior on angle {
                    NumberAnimation {
                        target: indicatorRot
                        property: "angle"
                        duration: 400
                        easing.type: Easing.OutCubic
                    }
                }
            }
        }
    }
}
