import QtQuick 2.12
import "qrc:/stylesheet"

Rectangle {
    anchors.fill: parent
    anchors.margins: -2
    color: "transparent"

    border {
        color: Stylesheet.colors.white
        width: 2
    }
    radius: 2

    visible: parent.focus && parent.focusReason === Qt.TabFocusReason
}
