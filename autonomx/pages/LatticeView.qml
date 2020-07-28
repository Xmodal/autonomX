import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import ca.hexagram.xmodal.autonomx 1.0

import "qrc:/stylesheet"
import "../models"
import "../components/delegates"

ColumnLayout {
    id: latticeView

    property int genID: window.activeGeneratorIndex

    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 0

    // header
    Header {}

    // main content
    RowLayout {
        id: mainContent

        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 0

        // props
        property int ppc: 20            // pixels per cell, ie. how wide a cell square is in pixels. this is scaled by the zoom factor
        property int cols: 20
        property int rows: 20
        property int currRegionIndex: -1

        // manage selected region
        function switchSelectedRegion(index) {
            currRegionIndex = index;
            regions.rectSelected = index >= 0;
        }

        // convert ListElement to QRect
        function elementToRect(element) {
            return Qt.rect(element.colX, element.colY, element.colW, element.colH);
        }

        // model
        RegionModel {
            id: regionModel
        }

        // matrix zone
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            GeneratorLattice {
                generatorID: 0
                anchors.fill: parent
            }

            /*
            Rectangle {
                anchors.fill: parent
                color: Stylesheet.colors.black
            }

            // neuron grid
            ShaderEffect {
                anchors.fill: parent
                visible: !(genID < 0)

                // uniforms
                property real cw: width
                property real ch: height
                property int ppc: mainContent.ppc
                property int cols: mainContent.cols
                property int rows: mainContent.rows
                property rect selected: mainContent.currRegionIndex < 0 ? Qt.rect(-1, -1, -1, -1) : mainContent.elementToRect(regionModel.get(mainContent.currRegionIndex))
                property Image textureMap: Image { id: neuronGrid; source: "qrc:/assets/images/neurongrid_20x20.png" }

                // fragment shader
                fragmentShader: "qrc:/shaders/neuron_matrix.frag"
            }

            // I/O regions
            Item {
                id: regions

                property bool rectSelected: false

                width: mainContent.ppc * mainContent.cols
                height: mainContent.ppc * mainContent.rows
                x: parent.width/2 - width/2
                y: parent.height/2 - height/2

                MouseArea {
                    anchors.fill: parent
                    onClicked: mainContent.switchSelectedRegion(-1)
                }

                Repeater {
                    model: regionModel
                    Region {}
                }
            }

            // tooltip (shown when no generator is selected)
            Label {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: 320

                visible: genID < 0

                text: qsTr("Please select a generator from the Generator View to edit its I/O zones.")
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                font {
                    family: Stylesheet.fonts.main
                    pixelSize: 14
                }
                opacity: 0.4
            }
            */
        }

        // control zone
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                anchors.fill: parent
                color: Stylesheet.colors.darkGrey
            }
        }
    }
}
