import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/stylesheet"

Button {
    id: iconButton

    property real size: 40
    property string iconSource
    property color activeColor: Stylesheet.colors.generator

    background: Rectangle {
        id: background
        color: pressed || checked ? activeColor : Stylesheet.colors.white
        opacity: pressed ? 1 : (checked ? (hovered ? 0.75 : 0.4) : (hovered ? 0.15 : 0))
        implicitWidth: size
        implicitHeight: size
    }

    indicator: Image {
        source: iconSource
        anchors.centerIn: parent
        smooth: true
        mipmap: false
    }
}
