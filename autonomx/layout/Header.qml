import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0

import "qrc:/stylesheet"
import "../components/ui"

ColumnLayout {
    property string currentFileUri

    function basename(str) {
        return (str.slice(str.lastIndexOf("/")+1));
    }

    Layout.fillWidth: true
    spacing: 0
    z: 100

    // save/load bar
    Rectangle {
        id: serializer

        Layout.fillWidth: true
        Layout.preferredHeight: 30

        color: Stylesheet.colors.black

        // changes lost warning
        MessageDialog {
            text: "You are about to modify the current project. Any unsaved changes will be lost."
            informativeText: "Do you want to continue?"

            buttons: MessageDialog.Ok | MessageDialog.Cancel
        }

        // save dialog
        FileDialog {
            id: saveDialog
            title: "Save project file as..."
            nameFilters: ["autonomX save files (*.atnx)"]
            defaultSuffix: "atnx"
            fileMode: FileDialog.SaveFile

            onAccepted: {
                currentFileUri = currentFile
                appModel.saveProject(currentFileUri)
            }
        }

        // load dialog
        FileDialog {
            id: loadDialog
            title: "Load project file..."
            nameFilters: ["autonomX save files (*.atnx)"]
            defaultSuffix: "atnx"
            fileMode: FileDialog.OpenFile

            onAccepted: {
                currentFileUri = currentFile

                if (appModel.loadProject(currentFileUri)) {
                    activeGeneratorIndex = -1;
                }
            }
        }

        RowLayout {
            anchors.fill: parent

            GenericButton {
                neutralColor: Stylesheet.colors.black
                text: "New"
                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true
            }

            GenericButton {
                neutralColor: Stylesheet.colors.black
                text: "Load"
                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true

                onClicked: loadDialog.open()
            }

            GenericButton {
                neutralColor: Stylesheet.colors.black
                text: "Save"
                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true

                onClicked: {
                    if (!currentFileUri) saveDialog.open()
                    else {
                        if (appModel.saveProject(currentFileUri)) {
                            // TODO: successful save animation :)
                        }
                    }
                }
            }

            GenericButton {
                neutralColor: Stylesheet.colors.black
                text: "Save as"
                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true

                onClicked: saveDialog.open()
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Label {
            anchors.fill: parent
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            text: currentFileUri ? basename(currentFileUri) : '<no project loaded>'
            opacity: currentFileUri ? 1 : 0.5
            font: Stylesheet.fonts.text
        }
    }

    // generator bar
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

                text: activeGeneratorIndex < 0 ? "" : generatorModel.at(activeGeneratorIndex).name
                opacity: activeGeneratorIndex < 0 ? 0.4 : 1
                font: Stylesheet.fonts.text
            }

            // filler
            Item {
                Layout.fillWidth: true
            }

            // restart/reset buttons
            GenericButton {
                id: restartButton
                text: "Restart"
                visible: activeGeneratorIndex >= 0

                Layout.rightMargin: 5

                onClicked: generatorModel.at(activeGeneratorIndex).initialize()
            }

            GenericButton {
                id: resetParametersButton
                text: "Reset params"
                visible: activeGeneratorIndex >= 0

                Layout.rightMargin: 15

                onClicked: generatorModel.at(activeGeneratorIndex).resetParameters()
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
}
