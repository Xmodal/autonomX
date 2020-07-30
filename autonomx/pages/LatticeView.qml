import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import ca.hexagram.xmodal.autonomx 1.0

import "qrc:/stylesheet"

ColumnLayout {
    id: latticeView

    property int genID: window.activeGeneratorIndex

    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 0;

    // header
    Item {
        id: header

        Layout.fillWidth: true
        Layout.preferredHeight: 45

        // background
        Rectangle {
            anchors.fill: parent
            color: genID < 0 ? Stylesheet.colors.white : Stylesheet.colors.outputs[genID % Stylesheet.colors.outputs.length]
        }

        // back arrow
        Image {
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter

            source: "qrc:/assets/images/left-arrow.svg"

            MouseArea {
                anchors.fill: parent
                onClicked: router.currentIndex = 0
            }
        }

        // "Lattice view" label
        Label {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.horizontalCenter
            anchors.rightMargin: 20

            text: "Lattice view"
            font {
                family: Stylesheet.fonts.sub
                pixelSize: 13
                letterSpacing: 13 * 0.05
                capitalization: Font.AllUppercase
            }
            color: Stylesheet.colors.darkGrey
        }

        // Middle separator
        Rectangle {
            anchors.bottom: parent.bottom;
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 12

            width: 1; height: parent.height - anchors.topMargin
            color: Stylesheet.colors.darkGrey
        }

        // Generator name label
        Label {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.horizontalCenter
            anchors.leftMargin: 20

            text: genID < 0 ? "<no generator selected>" : generatorModel.at(genID).name
            color: Stylesheet.colors.darkGrey
        }
    }

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
                property Image textureMap: Image { id: neuronGrid; source: "qrc:/assets/images/neurongrid_20x20.png" }

                // fragment shader
                fragmentShader: "qrc:/shaders/neuron_matrix.frag"
            }

            // I/O zones
            Canvas {
                id: zoneDrawings
                anchors.fill: parent
                visible: !(genID < 0)

                // slots
                onWidthChanged: requestPaint()
                onHeightChanged: requestPaint()

                // props
                property int cols: mainContent.cols
                property int rows: mainContent.rows
                property real realWidth: mainContent.realWidth
                property real realHeight: mainContent.realHeight

                // I/O
                property color inputColor: Stylesheet.colors.inputColor
                property variant inputs: [
                    Qt.rect(2, 2, 4, 5)
                ]
                property color outputColor: Stylesheet.colors.outputColor
                property variant outputs: [
                    Qt.rect(10, 10, 2, 2)
                ]

                // painting code
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.reset();
                    ctx.strokeWidth = 1;

                    // props
                    var i;
                    var offset = {
                        x: width/2 - realWidth/2,
                        y: height/2 - realHeight/2
                    };
                    var coords;

                    // draw inputs
                    for (i = 0; i < inputs.length; i++) {
                        ctx.beginPath();
                        ctx.strokeStyle = inputColor;

                        coords = {
                            x: offset.x + (realWidth/cols*inputs[i].x),
                            y: offset.y + (realHeight/rows*inputs[i].y),
                            w: realWidth/cols*inputs[i].width,
                            h: realHeight/rows*inputs[i].height
                        };

                        ctx.strokeRect(coords.x, coords.y, coords.w, coords.h);
                    }

                    // draw outputs
                    for (i = 0; i < outputs.length; i++) {
                        ctx.beginPath();
                        ctx.strokeStyle = outputColor;

                        coords = {
                            x: offset.x + (realWidth/cols*outputs[i].x),
                            y: offset.y + (realHeight/rows*outputs[i].y),
                            w: realWidth/cols*outputs[i].width,
                            h: realHeight/rows*outputs[i].height
                        };

                        ctx.strokeRect(coords.x, coords.y, coords.w, coords.h);
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
