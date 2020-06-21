import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Style"

ColumnLayout {
    id: field

    property string labelText: "Label"
    property int fieldWidth: 200

    implicitWidth: fieldWidth
    implicitHeight: fieldLabel.height + 45
    spacing: 5
    Layout.alignment: Qt.AlignLeft | Qt.AlignTop

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
