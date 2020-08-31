import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../components/ui"

Item {
    id: header

    Layout.fillWidth: true
    Layout.preferredHeight: Stylesheet.headerHeight
    z: 100

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
        spacing: 0

        IconButton {
            id: genListToggler
            size: 40
            iconSource: "qrc:/assets/images/generator-list.svg"

            checkable: true
            checked: showGeneratorList
            onCheckedChanged: showGeneratorList = checked
        }

        // "Lattice view" label
        Label {
            Layout.alignment: Qt.AlignVCenter
            Layout.leftMargin: 20

            text: "Now editing"
            font: Stylesheet.fonts.label
        }

        Rectangle {
            Layout.alignment: Qt.AlignVCenter
            Layout.leftMargin: 6
            Layout.rightMargin: 6
            implicitWidth: 30
            implicitHeight: 1
            color: Stylesheet.colors.white
            opacity: activeGeneratorIndex < 0 ? 0.4 : 1
        }

        // Generator name label
        Label {
            Layout.alignment: Qt.AlignVCenter

            text: activeGeneratorIndex < 0 ? "<none>" : generatorModel.at(activeGeneratorIndex).name
            opacity: activeGeneratorIndex < 0 ? 0.4 : 1
            font: Stylesheet.fonts.text
        }

        // filler
        Item {
            Layout.fillWidth: true
        }

        IconButton {
            id: paramsToggler
            size: 40
            iconSource: "qrc:/assets/images/wrench.svg"

            checkable: true
            checked: showGeneratorSettings
            onCheckedChanged: showGeneratorSettings = checked
            enabled: activeGeneratorIndex >= 0
        }
    }
}
