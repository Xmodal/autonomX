import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import ca.hexagram.xmodal.autonomx 1.0

import "qrc:/stylesheet"
import "../components/ui"
import "../components/fields"
import "../components/util"

Item {
    id: latticeView

    // props
    property int generatorIndex: window.activeGeneratorIndex
    property GeneratorRegionSet inputModel: generatorIndex < 0 ? null : generatorModel.at(generatorIndex).inputModel
    property GeneratorRegionSet outputModel: generatorIndex < 0 ? null : generatorModel.at(generatorIndex).outputModel

    // pan/zoom properties
    property real ppc: 0.1 * zoom                       // pixels per cell, ie. how wide a cell square is in pixels. this is animated within QML (scaled by the zoom factor)
    property alias zoom: panManager.zoom                // in percents
    property alias zoomCoarse: panManager.zoomCoarse    // in percents
    property alias pan: panManager.pan                  // in pixels
    property alias isDraggingRegion: panManager.isDraggingRegion

    // backend read
    onGeneratorIndexChanged: {
        panManager.setFromLatticeView(generatorModel.at(generatorIndex));
        allowSnapW = false;
        allowSnapH = false;
    }

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

    // prevents snapping when changing generators
    // see code in #regions element below
    property alias allowSnapW: regions.allowSnapW
    property alias allowSnapH: regions.allowSnapH

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

    // PanManager function aliases
    property var zoomIn: panManager.zoomIn
    property var zoomOut: panManager.zoomOut
    property var resetView: panManager.resetView

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
        squareInPixels: ppc     // = zooming
        mask: Qt.vector4d(-1, -1, -1, -1)
        maskAlpha: 0.5
        pan: latticeView.pan

        function setMask(rect) {
            var element;

            // auto cancel if type is signed
            if (currRegion.type === -1) {
                return mask = Qt.vector4d(-1, -1, -1, -1);
            }

            // set selected to function arguments if applicable
            if (rect !== undefined) {
                return mask = Qt.vector4d(rect.x/ppc, rect.y/ppc, rect.width/ppc, rect.height/ppc);
            }

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
        }
    }

    // pan and zoom area
    PanManager {
        id: panManager
    }

    // I/O regions
    Item {
        id: regions
        visible: !(generatorIndex < 0)

        // panning
        transform: Translate {
            x: -pan.x
            y: -pan.y
        }

        property int latticeWidth: generatorIndex < 0 ? 20 : generatorModel.at(generatorIndex).latticeWidth
        property int latticeHeight: generatorIndex < 0 ? 20 : generatorModel.at(generatorIndex).latticeHeight
        property bool rectSelected: !(currRegion.index < 0)

        property bool allowSnapW: true
        property bool allowSnapH: true

        onLatticeWidthChanged: {
            if (!allowSnapW) return allowSnapW = true;
            snapAllRegions()
        }

        onLatticeHeightChanged: {
            if (!allowSnapH) return allowSnapH = true;
            snapAllRegions()
        }

        function snapAllRegions() {
            let i;

            for (i = 0; i < inputRepeater.count; i++) {
                inputRepeater.itemAt(i).snapToGrid("drag", ppc * latticeWidth, ppc * latticeHeight, latticeWidth, latticeHeight);
            }
            for (i = 0; i < outputRepeater.count; i++) {
                outputRepeater.itemAt(i).snapToGrid("drag", ppc * latticeWidth, ppc * latticeHeight, latticeWidth, latticeHeight);
            }
        }

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
               Math.floor((crossbeamArea.mouseX - regions.x + pan.x) / ppc),
               Math.floor((crossbeamArea.mouseY - regions.y + pan.y) / ppc));
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

    // zoom field
    RowLayout {
        spacing: 0
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 20
        }

        NumberField {
            id: zoomField
            visible: !(generatorIndex < 0)
            propName: "zoomCoarse"
            target: latticeView
            unit: "%"

            fieldWidth: 120
            labelText: "Zoom"
            incStep: 5

            // override this because we want to change the exponent here
            valueChangedHandler: function(newValue) {
                if (target) {
                    panManager.zoomExp = Math.log2(newValue / 100);
                }
            }
        }

        Item { Layout.fillWidth: true }

        GenericButton {
            visible: panManager.zoomExp !== 0 || panManager.panX !== 0 || panManager.panY !== 0
            id: resetView
            text: "Reset view"
            Layout.alignment: Qt.AlignBottom

            onClicked: {
                panManager.zoomExp = 0;
                panManager.panX = 0;
                panManager.panY = 0;
            }
        }
    }
}
