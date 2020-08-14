import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11

import "./stylesheet"
import "./pages"

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

    StackLayout {
        id: router
        anchors.fill: parent
        currentIndex: 0

        GeneratorView {}
        LatticeView {}
    }
}
