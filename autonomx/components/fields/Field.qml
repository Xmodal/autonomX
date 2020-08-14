import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

ColumnLayout {
    id: field

    property string labelText: "Label"
    property int fieldWidth: Stylesheet.field.initialWidth
    property bool deactivated: false
    property bool showLabel: true
    property bool fillHeight: false
    property Component fieldContent

    signal valueChanged(variant newValue)

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

    Loader {
        sourceComponent: fieldContent
        Layout.preferredWidth: fieldWidth
        Layout.preferredHeight: fillHeight ? -1 : 40
        Layout.fillHeight: fillHeight
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
