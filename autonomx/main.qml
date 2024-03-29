import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11

import "./stylesheet"
import "./layout"
import "./components/util"

/**
 * Main window of this application
 */
ApplicationWindow {
    id: window

    onFocusObjectChanged: {
        if (!activeFocusItem) {
            altPressed = shiftPressed = false;
        }

        // console.log(activeFocusItem)
    }

    property string lastMessageReceived: ""
    property int activeGeneratorIndex: 0

    // the meta model used to create the interface
    // will only change when the generator type changes
    property var metaModel: generatorModel.at(activeGeneratorIndex).meta
    onActiveGeneratorIndexChanged: {
        if (activeGeneratorIndex < 0) return;
        metaModel = generatorModel.at(activeGeneratorIndex).meta
    }

    // break metaModel property on app launch
    // since it will be managed by separate signal (see above)
    Component.onCompleted: {
        metaModel = metaModel
    }

    // UI switch flags
    property bool showGeneratorList: false
    property bool showGeneratorSettings: true

    // global field helper flags
    property bool altPressed: false
    property bool shiftPressed: false
    property bool spacePressed: false
    property bool editingTextField: false
    property alias allowSlideDrag: slideDragger.visible

    // saving stuff
    property alias saveManager: saveManager
    property alias currentFileName: saveManager.currentFileName

    // changes when
    onEditingTextFieldChanged: {
        if (editingTextField) deAlt(mainContent)
    }

    visible: true
    width: 1440
    height: 810
    title: (currentFileName.length > 0 ? "[" + currentFileName + "] " : "") + Qt.application.name + " - version " + Qt.application.version

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
    property var components: ({
        SubRack: Qt.createComponent("qrc:/components/racks/SubRack.qml"),

        SliderField: Qt.createComponent("qrc:/components/fields/SliderField.qml"),
        NumberField: Qt.createComponent("qrc:/components/fields/NumberField.qml"),
        SelectField: Qt.createComponent("qrc:/components/fields/SelectField.qml"),
        TextField: Qt.createComponent("qrc:/components/fields/TextField.qml"),
        AreaField: Qt.createComponent("qrc:/components/fields/AreaField.qml")
    })


    // background
    background: Rectangle {
        color: Stylesheet.colors.black
    }


    // shortcut manager
    ShortcutManager {}

    // main content
    FocusScope {
        anchors.fill: parent
        focus: true

        ColumnLayout {
            id: mainContent
            anchors.fill: parent
            spacing: 0

            Header {}

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    LatticeView {
                        id: latticeView
                    }

                    RackList {
                        visible: activeGeneratorIndex >= 0 && showGeneratorSettings
                    }
                }

                // as overlay
                GeneratorList {
                    enabled: showGeneratorList
                }
            }
        }

        // shift/alt press detection
        Keys.onPressed: {
            if (editingTextField) return
            // do not allow auto repeating on modifiers
            if (event.isAutoRepeat) return;

            // modifiers
            if (event.key === Qt.Key_Alt) altPressed = true
            if (event.key === Qt.Key_Shift) shiftPressed = true
            if (event.key === Qt.Key_Space) spacePressed = true
        }
        Keys.onReleased: {
            // do not allow auto repeating on modifiers
            if (event.isAutoRepeat) return;

            // special alt case
            if (event.key === Qt.Key_Alt) {
                altPressed = false
                allowSlideDrag = false
                restoreCursor()
            }

            // other modifiers
            if (event.key === Qt.Key_Shift) shiftPressed = false
            if (event.key === Qt.Key_Space) spacePressed = false
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

        onVisibleChanged: {
            if (visible)
                return overrideCursor(Qt.SizeHorCursor);
            restoreCursor();
        }

        property alias mouseArea: slideDragArea
        Drag.active: slideDragArea.drag.active

        MouseArea {
            id: slideDragArea
            anchors.fill: parent

            drag.target: parent
            drag.threshold: 0
            drag.smoothed: false

            onReleased: {
                window.allowSlideDrag = false
                parent.x = 0
                parent.y = 0
            }
        }
    }

    SaveManager {
        id: saveManager
    }
}
