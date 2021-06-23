import QtQuick 2.11

import "qrc:/stylesheet"

Rectangle {
    id: lineJoint

    property real size: 30
    property real thickness: 1
    property bool vertical: false

    width: vertical ? thickness : size
    height: vertical ? size : thickness
    color: Stylesheet.colors.white
}
