import QtQuick 2.0
import QtQuick.Window 2.2

Item {
    property real luminosity: 0.0
    property color lightColor: "red"

    width: 30; height: 30

    Image {
        anchors.fill: parent
        source: "assets/images/light-cue.png"
        x: 0; y: 0
    }

    Rectangle {
        width: 16; height: width
        x: 7; y: 7
        radius: width*0.5
        color: Qt.tint("black", Qt.rgba(lightColor.r, lightColor.g, lightColor.b, luminosity))
    }
}
