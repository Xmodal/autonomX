import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11

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
    width: 1440
    height: 720
    title: qsTr("autonomX 0.1.0")

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

    Shortcut {
        sequence: "PgUp"
        onActivated: {
            if (activeGeneratorIndex < 0) return;
            var i = activeGeneratorIndex - 1;
            if (i < 0) i++;
            activeGeneratorIndex = i;
        }
    }

    Shortcut {
        sequence: "PgDown"
        onActivated: {
            var i = activeGeneratorIndex + 1;
            if (i >= generatorModel.rowCount()) i--;
            activeGeneratorIndex = i;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Header {}

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

                Rectangle {
                    anchors.fill: parent
                    z: -1

                    color: Stylesheet.colors.darkGrey
                }

                enabled: activeGeneratorIndex >= 0 && showGeneratorSettings
                visible: enabled
                source: enabled ? "qrc:/layout/RackList.qml" : ""
            }
        }
    }
}
