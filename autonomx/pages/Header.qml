import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Item {
    id: header

    Layout.fillWidth: true
    Layout.preferredHeight: 40

    // background
    Rectangle {
        anchors.fill: parent

        color: Stylesheet.colors.white

        layer.enabled: true
        layer.effect: ShaderEffect {
            property real minAlpha: 0.85
            property real maxAlpha: 0.95
            property bool vertical: true
            property bool overlayColor: Stylesheet.colors.black

            fragmentShader: "qrc:/shaders/gradient_overlay.frag"
        }
    }

    // "Lattice view" label
    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 20

        text: "Lattice view"
        font: Stylesheet.fonts.label
    }

    // Generator name label
    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 20

        text: genID < 0 ? "<no generator selected>" : generatorModel.at(genID).name
        font: Stylesheet.fonts.text
    }
}
