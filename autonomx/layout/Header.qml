import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Item {
    id: header

    Layout.fillWidth: true
    Layout.preferredHeight: Stylesheet.headerHeight

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

    RowLayout {
        anchors.fill: parent

        // "Lattice view" label
        Label {
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.leftMargin: 20

            text: "Now editing"
            font: Stylesheet.fonts.label
        }

        // Generator name label
        Label {
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.leftMargin: 40

            text: generatorIndex < 0 ? "<no generator selected>" : generatorModel.at(generatorIndex).name
            font: Stylesheet.fonts.text
        }

        // filler
        Item {
            Layout.fillWidth: true
        }
    }
}
