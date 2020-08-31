import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

import "./stylesheet"
import "./layout"

/**
 * Main window of this application
 */
ApplicationWindow {
    id: window

    property string lastMessageReceived: ""
    property int activeGeneratorIndex: -1

    property bool showGeneratorList: true
    property bool showGeneratorSettings: true

    visible: true
    width: 1280
    height: 720
    title: qsTr("autonomX 0.1.0")

    // TODO: move into appropriate Rack component
    function handleMessageReceived(oscPath, oscArguments) {
        console.log("QML-Received OSC: " + oscPath + " " + oscArguments);
        lastMessageReceived = oscPath + " " + oscArguments;
    }

    function toggleFullscreen() {
        if (visibility === Window.FullScreen) {
            visibility = Window.AutomaticVisibility;
        } else {
            visibility = Window.FullScreen;
        }
    }

    function quitThisApp() {
        Qt.quit();
    }

    // add/delete generators
    function addGenerator() {
        appModel.createGenerator();
        activeGeneratorIndex = generatorModel.rowCount() - 1;
    }
    function deleteGenerator(index) {
        appModel.deleteGenerator(generatorModel.at(activeGeneratorIndex > 0 ? activeGeneratorIndex : index).id);
        if (activeGeneratorIndex === generatorModel.rowCount()) activeGeneratorIndex--;
    }

    // background
    background: Rectangle {
        color: Stylesheet.colors.black
    }

    // Shortcuts:
    Shortcut {
        sequence: "Esc"
        onActivated: toggleFullscreen()
    }

    Shortcut {
        sequence: "Ctrl+Q"
        onActivated: quitThisApp()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Header {
            z: 100
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            GeneratorList {
                enabled: showGeneratorList
                visible: enabled
            }

            LatticeView {}

            Loader {
                id: rackViewLoader
                Layout.preferredWidth: 590
                Layout.fillHeight: true

                enabled: activeGeneratorIndex >= 0 && showGeneratorSettings
                visible: enabled
                source: enabled ? "qrc:/layout/RackList.qml" : ""
            }
        }
    }
}
