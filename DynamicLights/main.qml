import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11

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

    function toggleDebugView() {
        stackLayout0.currentIndex = (stackLayout0.currentIndex + 1) % 2;
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
        spacing: 6

        // List of generators
        ListView {
            Layout.margins: 0
            Layout.fillWidth: false
            Layout.fillHeight: true
            orientation: Qt.Vertical
            width: currentItem.width
            model: generatorsModel

            delegate: GeneratorWidget {
                generatorName: name
                generatorIndex: index + 1
                height: parent.height / parent.count
                spacing: 0
            }
        }

        // Contents
        StackLayout {
            id: stackLayout0
            currentIndex: 0
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 0

                StackLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: 0

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        RowLayout {
                            Layout.alignment: Qt.AlignRight
                            Layout.fillWidth: false
                            Layout.fillHeight: false

                            Button {
                                text: "Click Me"
                            }
                            Button {
                                text: "Click Me Too"
                            }
                        }
                    }
                }
            }

            // OSC debug layout:
            ColumnLayout {
                RowLayout {
                    SpinBox {
                        id: someInt
                        value: 2
                    }
                    Slider {
                        id: someDouble
                        value: 3.14159
                        from: 0.0
                        to: 5.0
                    }
                    TextField {
                        id: someText
                        text: "hello"
                    }
                }

                Button {
                    text: "Send OSC"
                    onClicked: {
                        oscSender.send("/hello", [someInt.value, someDouble.value, someText.text]);
                    }
                }

                RowLayout {
                    Label {
                        text: "Received:"
                    }
                    Label {
                        text: lastMessageReceived
                    }
                }
            }
        }
    }


}
