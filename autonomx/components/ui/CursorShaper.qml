import QtQuick 2.11
import QtQuick.Controls 2.2

// you have no idea of how frustrating this is for me
// like, couldn't they just include the alias out of the box??? why??????
// :(
MouseArea {
    anchors.fill: parent
    onPressed: mouse.accepted = false
    cursorShape: Qt.PointingHandCursor
}
