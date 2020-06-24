import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Style"

ColumnLayout {
    id: field

    property string labelText: "Label"
    property int fieldWidth: Stylesheet.field.initialWidth

    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    Layout.maximumWidth: fieldWidth
    Layout.fillWidth: true

    spacing: 5

    // top label
    Label {
        id: fieldLabel

        text: labelText
        color: Stylesheet.colors.white
        font {
            family: Stylesheet.fonts.sub
            pixelSize: 14
            capitalization: Font.AllUppercase
        }
    }
}
