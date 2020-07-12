import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

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
            color: Stylesheet.colors.outputs[genID % Stylesheet.colors.outputs.length]
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

            text: generatorModel.at(genID).name
            color: Stylesheet.colors.darkGrey
        }
    }

    // main content
    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        spacing: 0

        // matrix zone
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                anchors.fill: parent
                color: Stylesheet.colors.black
            }

            ShaderEffect {
                anchors.fill: parent

                property real cw: width
                property real ch: height
                property int cols: 20
                property int rows: 20
                property int show: 1

                property variant inputs: [
                    Qt.rect(2, 4, 4, 5)
                ]
                property variant outputs: [
                    Qt.rect(10, 10, 2, 5)
                ]

                property Image textureMap: Image { id: neuronGrid; source: "qrc:/assets/images/neurongrid_20x20.png" }

                fragmentShader: "qrc:/shaders/neuron_matrix.frag"
            }
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
