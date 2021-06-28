import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import ca.hexagram.xmodal.autonomx 1.0

import "qrc:/stylesheet"
import "../components/ui"

Item {
    id: latticeView

    // props
    property int generatorIndex: window.activeGeneratorIndex
    property GeneratorRegionSet inputModel: generatorIndex < 0 ? null : generatorModel.at(generatorIndex).getInputRegionModel()
    property GeneratorRegionSet outputModel: generatorIndex < 0 ? null : generatorModel.at(generatorIndex).getOutputRegionModel()
    property int ppc: 20            // pixels per cell, ie. how wide a cell square is in pixels. this is animated within QML (scaled by the zoom factor)

    property QtObject currRegion: QtObject {
        property int type: -1
        property int index: -1
        property bool adding: false
    }

    property alias inputsVisible: showInputsBtn.checked
    property alias outputsVisible: showOutputsBtn.checked

    property alias ioToolbar: ioToolbar

    // layout
    Layout.fillWidth: true
    Layout.fillHeight: true
    z: -1

    // manage selected region
    function switchSelectedRegion(type, index, adding = false) {
        const rowCount = generatorModel.at(generatorIndex)[type ? "outputCount" : "inputCount"];

        // change current region props
        currRegion.type = type;
        currRegion.index = adding ? index : Math.max(-1, index);
        currRegion.adding = adding;

        // set lattice mask
        if (!adding)
            matrix.setMask();
        else
            matrix.setMask(Qt.rect(0, 0, 0, 0));
    }

    // bound validator
    function isWithinBounds() {
        return !(currRegion.type < 0);
    }

    // navigation
    function goToNextRegion() {
        if (!isWithinBounds()) return;
        return switchSelectedRegion(currRegion.type, currRegion.index + 1, false);
    }
    function goToPrevRegion() {
        if (!isWithinBounds()) return;
        return switchSelectedRegion(currRegion.type, currRegion.index - 1, false);
    }
    function switchCurrRegionType() {
        if (currRegion.type < 0) return;
    }

    // add region
    function addNewInput() {
        switchSelectedRegion(0, inputModel.rowCount(), true);
    }
    function addNewOutput() {
        switchSelectedRegion(1, outputModel.rowCount(), true);
    }

    // delete currently selected region
    function deleteCurrentRegion() {
        if (!isWithinBounds()) return;

        let queuedIndex = currRegion.index;
        let destIndex;

        // input
        if (currRegion.type === 0 && inputModel) {
            destIndex = Math.min(queuedIndex, inputModel.rowCount() - 2);
            inputModel.deleteRegion(queuedIndex);

            if (inputModel.rowCount() === 0)
                return switchSelectedRegion(-1, -1);
        }

        // output
        else if (currRegion.type === 1 && outputModel) {
            destIndex = Math.min(queuedIndex, outputModel.rowCount() - 2);
            outputModel.deleteRegion(queuedIndex);

            if (outputModel.rowCount() === 0)
                return switchSelectedRegion(-1, -1);
        }

        switchSelectedRegion(currRegion.type, destIndex);
    }

    Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors.black
    }

    GeneratorLattice {
        id: matrix

        generatorID: generatorIndex < 0 ? -1 : generatorModel.at(generatorIndex).id
        anchors.fill: parent

        // uniforms
        squareInPixels: ppc
        mask: Qt.vector4d(-1, -1, -1, -1)
        maskAlpha: 0.3

        function setMask(rect) {
            var element;

            // auto cancel if type is signed
            if (currRegion.type === -1) return mask = Qt.vector4d(-1, -1, -1, -1);

            // set selected to function arguments if applicable
            if (rect !== undefined) return mask = Qt.vector4d(rect.x, rect.y, rect.width, rect.height);

            // otherwise, retrieve automatically from global properties
            // TODO: clean up function calls so that this block can be removed, instead directly using arguments every time
            if (currRegion.type === 0 && currRegion.index < inputModel.rowCount()) element = inputModel.at(currRegion.index);
            else if (currRegion.type === 1 && currRegion.index < outputModel.rowCount()) element = outputModel.at(currRegion.index);
            mask = Qt.vector4d(element.rect.x, element.rect.y, element.rect.width, element.rect.height);
        }

        // TODO
        /*
        NumberAnimation on maskAlpha {
            duration: 300
            easing.type: Easing.InOutQuad
        }
        */
    }

    MouseArea {
        id: matrixMouseBg
        enabled: !(generatorIndex < 0)
        anchors.fill: parent
        onClicked: {
            latticeView.switchSelectedRegion(-1, -1);
            mainContent.forceActiveFocus()
            showGeneratorList = false;
        }
    }

    // I/O regions
    Item {
        id: regions
        visible: !(generatorIndex < 0)

        property int latticeWidth: generatorIndex < 0 ? 20 : generatorModel.at(generatorIndex).latticeWidth
        property int latticeHeight: generatorIndex < 0 ? 20 : generatorModel.at(generatorIndex).latticeHeight
        property bool rectSelected: !(currRegion.index < 0)

        width: ppc * latticeWidth
        height: ppc * latticeHeight
        x: latticeView.width/2 - width/2
        y: latticeView.height/2 - height/2

        FocusScope {
            // inputs
            Repeater {
                id: inputRepeater
                model: inputModel

                Region {
                    type: 0
                    visible: inputsVisible
                }
            }
            // outputs
            Repeater {
                id: outputRepeater
                model: outputModel

                Region {
                    type: 1
                    visible: outputsVisible
                }
            }
        }

        // dummy added region
        DummyRegion {
            id: dummyRegion
            visible: currRegion.adding && crossbeamArea.containsMouse

            // start/end corner anchors
            // bindings altered by crossbeamArea > onPressedChanged
            start: { crossbeamArea.mouseToCoords() }
            end: { crossbeamArea.mouseToCoords() }
        }
    }

    // visible toggles
    ColumnLayout {
        id: displayTool
        visible: !(generatorIndex < 0) && !currRegion.adding
        spacing: 10

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 20

        Image {
            source: "qrc:/assets/images/eye.svg"
            width: 16
            height: 9

            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            width: 25
            height: 1
            color: Stylesheet.colors.white
            opacity: 0.5

            Layout.alignment: Qt.AlignHCenter
        }

        IOButton {
            id: showInputsBtn
            type: 0
            checkable: true
            checked: true

            Layout.alignment: Qt.AlignHCenter
        }

        IOButton {
            id: showOutputsBtn
            type: 1
            checkable: true
            checked: true

            Layout.alignment: Qt.AlignHCenter
        }
    }

    // tooltip (shown when no generator is selected)
    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: 320

        visible: generatorIndex < 0

        text: qsTr("Select a generator from the left panel to visualize and edit it.")
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 14
        opacity: 0.4
    }

    // region creation area
    MouseArea {
        id: crossbeamArea

        // start/end mouse positions
        // determines the upper-left and down-right corners of a region
        // TODO: ability to create a region in negatives axes as well
        property point mouseStartPos
        property point mouseEndPos

        property bool outOfBounds: false

        // compute mouse to lattice coords
        // see DummyRegion above for usage
        function mouseToCoords() {
            return Qt.point(
               Math.floor((crossbeamArea.mouseX - regions.x) / ppc),
               Math.floor((crossbeamArea.mouseY - regions.y) / ppc));
        }

        anchors.fill: parent
        enabled: currRegion.adding
        visible: enabled
        hoverEnabled: true

        // manage crossbeam cursor
        onContainsMouseChanged: {
            if (containsMouse)
                return overrideWithBeam();
            restoreCursor();
        }
        onEnabledChanged: restoreCursor()

        // manage press drag actions
        onPressedChanged: {
            if (pressed) {
                // don't do anything if initial selection is out of bounds;
                // also display a warning message, maybe
                if (dummyRegion.start.x < 0 || dummyRegion.start.x >= regions.latticeWidth || dummyRegion.start.y < 0 || dummyRegion.start.y >= regions.latticeHeight)
                    return outOfBounds = true;

                // unbind start position
                dummyRegion.start = dummyRegion.start;
                // lock inside bounds
                dummyRegion.dragging = true;
            } else {
                // don't do anything when out of bounds
                if (outOfBounds) return outOfBounds = false;

                // create new region based on new coordinates
                if (currRegion.type === 0) {
                    inputModel.addRegion(dummyRegion.area);
                } else {
                    outputModel.addRegion(dummyRegion.area);
                }

                // re-bind start/end position
                dummyRegion.start = Qt.binding(mouseToCoords)
                // unlock bounds
                dummyRegion.dragging = false;

                // batch add regions on Shift hold
                if (shiftPressed)
                    switchSelectedRegion(currRegion.type, currRegion.index + 1, true)
            }
        }
    }

    // I/O toolbar
    IOToolbar {
        id: ioToolbar
        visible: generatorIndex >= 0
    }
}
