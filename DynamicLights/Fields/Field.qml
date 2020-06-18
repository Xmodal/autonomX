import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

import "../Style"

ColumnLayout {
    id: field

    property string labelText: "Label"
    property alias fieldContent: fieldLoader.sourceComponent

    spacing: 5

    // top label
    Label {
        id: fieldLabel

        text: labelText
        color: Stylesheet.colors.white
        font {
            family: Stylesheet.fonts.sub
            pointSize: 11
            capitalization: Font.AllUppercase
        }
    }

    // field
    Loader { id: fieldLoader }
}
