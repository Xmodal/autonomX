import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Style"

ColumnLayout {
    id: field

    property string labelText: "Label"
    property int fieldWidth: Stylesheet.field.initialWidth
    property bool deactivated: false

    signal valueChanged(variant newValue)

    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    Layout.preferredWidth: fieldWidth
    Layout.maximumWidth: fieldWidth

    spacing: 5

    // top label
    Label {
        id: fieldLabel

        text: labelText
        font {
            family: Stylesheet.fonts.sub
            pixelSize: 13
            letterSpacing: 13 * 0.05
            capitalization: Font.AllUppercase
        }
    }

    // animations
    states: State {
        name: "deactivated"; when: field.deactivated
        PropertyChanges { target: field; opacity: 0.25 }
    }
    Behavior on opacity {
        NumberAnimation {
            property: "opacity"
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }
}
