import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../components/ui"

ColumnLayout {

    Layout.fillWidth: true
    spacing: 0
    z: 100

    // save/load bar
    Rectangle {
        id: serializer

        Layout.fillWidth: true
        Layout.preferredHeight: 30

        color: Stylesheet.colors.black

        RowLayout {
            anchors.fill: parent

            // TODO: the C++ function for this button
//            GenericButton {
//                neutralColor: Stylesheet.colors.black
//                text: "New"
//                Layout.alignment: Qt.AlignLeft
//                Layout.fillHeight: true
//            }

            GenericButton {
                neutralColor: Stylesheet.colors.black
                text: "Load"
                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true

                onClicked: saveManager.load()
            }

            GenericButton {
                neutralColor: Stylesheet.colors.black
                text: "Save"
                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true

                onClicked: saveManager.save()
            }

            GenericButton {
                neutralColor: Stylesheet.colors.black
                text: "Save as"
                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true

                onClicked: saveManager.saveAs()
            }

            Item {
                Layout.fillWidth: true
            }

            OscSettings {
                Layout.fillWidth: false
                Layout.fillHeight: true
            }
        }

        Label {
            anchors.fill: parent
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            text: currentFileName ? currentFileName : '<no project loaded>'
            opacity: currentFileName ? 1 : 0.5
            font: Stylesheet.fonts.text
        }
    }

    // generator bar
    Item {
        id: header
        z: -1

        Layout.fillWidth: true
        Layout.preferredHeight: Stylesheet.headerHeight

        Rectangle {
            anchors.fill: parent
            color: Stylesheet.colors.white

            layer.enabled: true
            layer.effect: ShaderEffect {
                property real minAlpha: 0.85
                property real maxAlpha: 0.95
                property bool vertical: false
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

                // right border
                Rectangle {
                    width: 1
                    height: parent.height - 1
                    color: Stylesheet.colors.white
                    opacity: 0.25
                    anchors.right: parent.right
                }
            }

            // "Lattice view" label
            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: 20

                text: activeGeneratorIndex < 0 ? "No selection" : "Now editing"
                font: Stylesheet.fonts.label
            }

            Rectangle {
                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: 6
                Layout.rightMargin: 6
                implicitWidth: 30
                implicitHeight: 1
                color: Stylesheet.colors.white
                visible: activeGeneratorIndex >= 0
            }

            // Generator name label
            Label {
                Layout.alignment: Qt.AlignVCenter

                text: activeGeneratorIndex < 0 ? "" : generatorModel.at(activeGeneratorIndex).generatorName
                opacity: activeGeneratorIndex < 0 ? 0.4 : 1
                font: Stylesheet.fonts.fieldText
            }

            // filler
            Item {
                Layout.fillWidth: true
            }

            // generator type label
            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.rightMargin: 15

                text: activeGeneratorIndex < 0 ? "" : generatorModel.at(activeGeneratorIndex).type
                opacity: 1
                font: Stylesheet.fonts.label
            }

            // restart/reset buttons
            RowLayout {
                spacing: 5
                Layout.rightMargin: 15
                visible: activeGeneratorIndex >= 0

                GenericButton {
                    id: restartButton
                    text: "Restart"
                    onClicked: generatorModel.at(activeGeneratorIndex).initialize()
                }

                GenericButton {
                    id: resetParametersButton
                    text: "Reset params"
                    onClicked: generatorModel.at(activeGeneratorIndex).resetParameters()
                }

                GenericButton {
                    id: resetRegionsButton
                    text: "Reset regions"
                    onClicked: generatorModel.at(activeGeneratorIndex).resetRegions()
                }
            }

            IconButton {
                id: paramsToggler
                size: 40
                iconSource: "qrc:/assets/images/wrench.svg"

                checkable: true
                checked: showGeneratorSettings
                onCheckedChanged: showGeneratorSettings = checked
                enabled: activeGeneratorIndex >= 0

                // left border
                Rectangle {
                    width: 1
                    height: parent.height - 1
                    color: Stylesheet.colors.white
                    opacity: 0.25
                    anchors.left: parent.left
                }
            }
        }

        // bottom border
        Rectangle {
            width: parent.width
            height: 1
            color: Stylesheet.colors.white
            opacity: 0.25
            anchors.bottom: parent.bottom
        }
    }
}
