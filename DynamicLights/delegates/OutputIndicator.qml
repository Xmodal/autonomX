import QtQuick 2.0
import QtQuick.Window 2.2

Item {
    property real luminosity: 0.0
    property color lightColor: "red"

    width: 30; height: 30

    Rectangle {
        anchors.fill: parent
        color: "#393939"
        radius: 15

        layer.enabled: true
        layer.effect: ShaderEffect {
            fragmentShader: "qrc:/shaders/light-cue.frag"
        }
    }

    Rectangle {
        width: 16; height: width
        x: 7; y: 7
        radius: width*0.5
        color: Qt.tint("black", Qt.rgba(lightColor.r, lightColor.g, lightColor.b, luminosity))
    }
}
