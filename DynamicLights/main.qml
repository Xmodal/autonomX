import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11

import "./Style"
import "./Racks"

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

    // this function could be useful in the future
    // so i'll keep it defined
    function toggleDebugView() {
        //stackLayout0.currentIndex = (stackLayout0.currentIndex + 1) % 2;
    }

    // background
    background: Rectangle {
        color: Stylesheet.colors.darkGrey
    }

    // TODO: move this in OSC rack
    Connections {
        target: oscReceiver

        onMessageReceived: {
            handleMessageReceived(oscAddress, message);
        }
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

    // auto input blur
    MouseArea {
        anchors.fill: parent
        onClicked: forceActiveFocus()
    }

    // Main two-column layout
    RowLayout {
        anchors.fill: parent
        spacing: 0

        // List of generators
        Item {
            Layout.preferredWidth: parent.width * 0.25
            Layout.minimumWidth: 200
            Layout.maximumWidth: 400
            Layout.fillHeight: true

            Rectangle {
                color: Stylesheet.colors.black
                anchors.fill: parent
            }

            ListView {
                anchors.fill: parent
                orientation: Qt.Vertical
                model: generatorModel
                delegate: GeneratorWidget {}
            }

            // TODO: new generator button here
        }

        // List of racks for currently selected generator
        RackView {
            id: rackView
        }
    }
}
