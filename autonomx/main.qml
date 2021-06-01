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

    // the meta model used to create the interface
    // will only change when the generator type changes
    property var metaModel
    onActiveGeneratorIndexChanged: {
        if (activeGeneratorIndex < 0) return;
        metaModel = generatorModel.at(activeGeneratorIndex).meta
    }

    property bool showGeneratorList: true
    property bool showGeneratorSettings: true

    // We update this version number each time we do a release/tag.
    // We follow Semantic Versioning 2.0.0 https://semver.org/
    readonly property string softwareVersion: "0.1.1-SNAPSHOT"

    property bool altPressed: false
    property bool shiftPressed: false
    property bool editingTextField: false
    property alias allowSlideDrag: slideDragger.visible

    onEditingTextFieldChanged: {
        if (editingTextField) deAlt(mainContent)
    }

    visible: true
    width: 1440
    height: 720
    title: qsTr("autonomX") + " " + softwareVersion

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
    function addGenerator(type) {
        appModel.createGenerator(type);
        activeGeneratorIndex = generatorModel.rowCount() - 1;
    }
    function deleteGenerator(index) {
        appModel.deleteGenerator(generatorModel.at(activeGeneratorIndex > 0 ? activeGeneratorIndex : index).id);
        if (activeGeneratorIndex === generatorModel.rowCount()) activeGeneratorIndex--;
    }


    // component creation
    // used by ParametersRack to dynamically generate field structure
    // from field tree provided by each generator type
    property var components
    function registerComponents() {
        components = {
            SubRack: Qt.createComponent("qrc:/components/racks/SubRack.qml"),

            SliderField: Qt.createComponent("qrc:/components/fields/SliderField.qml"),
            NumberField: Qt.createComponent("qrc:/components/fields/NumberField.qml"),
            SelectField: Qt.createComponent("qrc:/components/fields/SelectField.qml"),
            TextField: Qt.createComponent("qrc:/components/fields/TextField.qml"),
            AreaField: Qt.createComponent("qrc:/components/fields/AreaField.qml")
        };
    }

    Component.onCompleted: registerComponents()


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

    // main content
    ColumnLayout {
        id: mainContent
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

            RackList {
                visible: activeGeneratorIndex >= 0 && showGeneratorSettings
            }
        }

        // shift/alt press detection
        Keys.onPressed: {
            if (editingTextField) return

            if (event.key === Qt.Key_Alt) altPressed = true
            if (event.key === Qt.Key_Shift) shiftPressed = true
        }
        Keys.onReleased: {
            if (event.key === Qt.Key_Alt) {
                altPressed = false
                allowSlideDrag = false
                restoreCursor()
            }

            if (event.key === Qt.Key_Shift) shiftPressed = false
        }
    }

    // slide drag detection/mgmt
    Item {
        id: slideDragger
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        visible: false

        property alias mouseArea: dragArea
        Drag.active: dragArea.drag.active

        MouseArea {
            id: dragArea
            anchors.fill: parent

            drag.target: parent
            drag.threshold: 0
            drag.smoothed: false

            cursorShape: Qt.SizeHorCursor

            onReleased: {
                window.allowSlideDrag = false
                parent.x = 0
                parent.y = 0
            }
        }
    }
}
