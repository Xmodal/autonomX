import QtQuick 2.11
import QtQuick.Controls 2.2

import "qrc:/stylesheet"

Button {
    id: iconButton

    property real size: 40
    property string iconSource
    property color activeColor: Stylesheet.colors.generator

    background: Rectangle {
        id: background
        color: checked ? Stylesheet.colors.white : activeColor
        opacity: hovered ? (pressed ? 0.75 : 1) : (pressed || checked ? 1.0 : 0)
        implicitWidth: size
        implicitHeight: size
    }

    indicator: Image {
        source: iconSource
        anchors.centerIn: parent
        smooth: true
        mipmap: false

        opacity: hovered ? 1 : 0.5

        layer.enabled: true
        layer.effect: ShaderEffect {
            property bool checked: iconButton.checked
            fragmentShader: "qrc:/shaders/icon_button.frag"
        }
    }

    CursorShaper {}
}
