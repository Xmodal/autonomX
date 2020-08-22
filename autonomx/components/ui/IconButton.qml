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
        color: checked ? Stylesheet.colors.white : activeColor
        opacity: hovered ? (pressed ? 0.75 : 1) : (pressed || checked ? 0.75 : 0)
        implicitWidth: size
        implicitHeight: size
    }

    indicator: Image {
        source: iconSource
        anchors.centerIn: parent
        smooth: true
        mipmap: false

        layer.enabled: true
        layer.effect: ShaderEffect {
            property bool checked: iconButton.checked
            fragmentShader: "qrc:/shaders/icon_button.frag"
        }
    }
}
