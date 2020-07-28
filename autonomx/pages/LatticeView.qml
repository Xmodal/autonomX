import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import ca.hexagram.xmodal.autonomx 1.0

import "qrc:/stylesheet"
import "../models"

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

        // this will eventually change
        // since real width/height will depend on zoom factor
        // same thing goes for panning
        property real realWidth: 400
        property real realHeight: 400
        property int cols: 20
        property int rows: 20
        property int currRegionIndex: -1

        // manage selected region
        function switchSelectedRegion(index) {
            currRegionIndex = index;
            regions.rectSelected = index >= 0;
        }

        function elementToRect(element) {
            return Qt.rect(element.colX, element.colY, element.colW, element.colH);
        }

        RegionModel {
            id: regionModel
        }

        spacing: 0

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
                property real realWidth: mainContent.realWidth
                property real realHeight: mainContent.realHeight
                property int cols: neuronGrid.sourceSize.width
                property int rows: neuronGrid.sourceSize.height
                property rect selected: mainContent.currRegionIndex < 0 ? Qt.rect(-1, -1, -1, -1) : mainContent.elementToRect(regionModel.get(mainContent.currRegionIndex))
                property Image textureMap: Image { id: neuronGrid; source: "qrc:/assets/images/neurongrid_20x20.png" }

                // fragment shader
                fragmentShader: "qrc:/shaders/neuron_matrix.frag"
            }

            // I/O regions
            Item {
                id: regions

                property int latticeW: 400
                property int latticeH: 400
                property bool rectSelected: false

                x: parent.width/2 - width/2
                y: parent.height/2 - height/2
                width: latticeW
                height: latticeH

                MouseArea {
                    anchors.fill: parent
                    onClicked: mainContent.switchSelectedRegion(-1)
                }

                Repeater {
                    model: regionModel

                    Rectangle {
                        property bool selected: index === mainContent.currRegionIndex

                        antialiasing: true
                        width: regions.latticeW * colW / 20.0
                        height: regions.latticeH * colH / 20.0
                        x: regions.latticeW * colX / 20.0
                        y: regions.latticeH * colY / 20.0

                        opacity: regions.rectSelected && !selected ? 0.2 : 1

                        color: "transparent"
                        border {
                            color: type == 0 ? Stylesheet.colors.input : Stylesheet.colors.output
                            width: 1
                        }

                        // drag area
                        MouseArea {
                            anchors.fill: parent
                            onClicked: mainContent.switchSelectedRegion(index)
                            cursorShape: Qt.SizeAllCursor
                        }
                    }
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
