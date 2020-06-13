import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11

import DynamicLights.Style 1.0
import DynamicLights.Racks 1.0

/**
 * Main window of this application
 */
ApplicationWindow {
    id: window

    property string lastMessageReceived: ""

    visible: true
    width: 1280
    height: 720
    title: qsTr("Dynamic Lights")

    // background
    background: Rectangle {
        color: Stylesheet.colors.darkGrey
    }

    // OSC delegate
    // TODO: move into appropriate Rack component
    function handleMessageReceived(oscPath, oscArguments) {
        console.log("QML-Received OSC: " + oscPath + " " + oscArguments);
        lastMessageReceived = oscPath + " " + oscArguments;
    }

    Connections {
        target: oscReceiver

        onMessageReceived: {
            handleMessageReceived(oscAddress, message);
        }
    }

    /**
     * Toggles the fullscreen state of the main window.
     */
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

    // this function could be useful in the future
    // so i'll keep it defined
    function toggleDebugView() {
//        stackLayout0.currentIndex = (stackLayout0.currentIndex + 1) % 2;
    }

    // Models:
    GeneratorsModel {
        id: generatorsModel
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
        onActivated: toggleDebugView()
    }

    // Main two-column layout
    RowLayout {
        anchors.fill: parent
        spacing: 0

        // List of generators
        ListView {
            Layout.margins: 0
            Layout.fillHeight: true
            orientation: Qt.Vertical
            width: window.width * (3 / 12)
            model: generatorsModel

            delegate: GeneratorWidget {
                generatorName: name
                generatorIndex: index + 1
            }
        }

        // List of racks for currently selected generator
        RackView {}
    }
}
