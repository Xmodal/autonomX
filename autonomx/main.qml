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

    visible: true
    width: 1280
    height: 720
    title: qsTr("autonomX")

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

    function toggleLatticeView() {
        router.currentIndex = (router.currentIndex + 1) % 2;
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
    Shortcut {
        sequence: "Tab"
        onActivated: toggleLatticeView();
    }

    Item {
        anchors.fill: parent

        LatticeView {}

        GeneratorList {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
        }

        Loader {
            id: rackViewLoader
            width: 590
            height: parent.height - Stylesheet.headerHeight
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            visible: !(window.activeGeneratorIndex < 0)
            enabled: !(window.activeGeneratorIndex < 0)

            source: window.activeGeneratorIndex < 0 ? "" : "qrc:/layout/RackList.qml"
        }
    }
}
