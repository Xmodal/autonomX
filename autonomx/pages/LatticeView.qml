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

            // I/O regions
            Item {
                id: regions

                property int latticeW: 400
                property int latticeH: 400

                x: parent.width/2 - width/2
                y: parent.height/2 - height/2
                width: latticeW
                height: latticeH

                Repeater {
                    model: ListModel {
                        ListElement {
                            colX: 1
                            colY: 3
                            colW: 3
                            colH: 3
                            type: 0
                        }

                        ListElement {
                            colX: 6
                            colY: 3
                            colW: 3
                            colH: 3
                            type: 0
                        }

                        ListElement {
                            colX: 11
                            colY: 3
                            colW: 3
                            colH: 3
                            type: 0
                        }

                        ListElement {
                            colX: 16
                            colY: 3
                            colW: 3
                            colH: 3
                            type: 0
                        }

                        ListElement {
                            colX: 1
                            colY: 14
                            colW: 3
                            colH: 3
                            type: 1
                        }

                        ListElement {
                            colX: 6
                            colY: 14
                            colW: 3
                            colH: 3
                            type: 1
                        }

                        ListElement {
                            colX: 11
                            colY: 14
                            colW: 3
                            colH: 3
                            type: 1
                        }

                        ListElement {
                            colX: 16
                            colY: 14
                            colW: 3
                            colH: 3
                            type: 1
                        }
                    }

                    Rectangle {
                        antialiasing: true
                        width: regions.latticeW * model.colW / 20.0
                        height: regions.latticeH * model.colH / 20.0
                        x: regions.latticeW * model.colX / 20.0
                        y: regions.latticeH * model.colY / 20.0

                        color: "transparent"
                        border {
                            color: type == 0 ? Stylesheet.colors.input : Stylesheet.colors.output
                            width: 1
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
