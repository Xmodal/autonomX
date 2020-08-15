import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../ui"

ColumnLayout {
    id: field

    property string labelText: "Label"
    property int fieldWidth: Stylesheet.field.initialWidth
    property bool showLabel: true
    property bool fillHeight: false
    property bool enableFlag: false
    property bool flagValue
    property bool deactivated: false
    property Component fieldContent

    signal valueChanged(variant newValue)
    signal flagChanged(bool newFlag)

    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    Layout.preferredWidth: fieldWidth
    Layout.fillHeight: fillHeight

    spacing: 5

    opacity: deactivated ? 0.25 : 1

    // top label
    Label {
        id: fieldLabel
        visible: showLabel

        text: labelText
        font: Stylesheet.fonts.label
    }

    RowLayout {
        spacing: 0
        Layout.maximumWidth: fieldWidth
        Layout.preferredHeight: 40
        Layout.fillWidth: true
        Layout.fillHeight: fillHeight

        // flag
        Item {
            visible: enableFlag
            Layout.preferredWidth: 30
            Layout.fillHeight: true

            CheckBox {}
            Image {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                source: "qrc:/assets/images/check-frame.svg"
                opacity: 0.3
            }
        }

        // field
        Loader {
            sourceComponent: fieldContent
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    // animations
    Behavior on opacity {
        NumberAnimation {
            property: "opacity"
            duration: 250
            easing.type: Easing.InOutQuad
        }
    }
}
