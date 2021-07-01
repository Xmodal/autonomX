import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import "qrc:/stylesheet"
import "../components/ui"

Item {
    id: ioToolbar

    property int selectedType: latticeView.currRegion.type            // 0 = input;  1 = output
    property int selectedIndex: latticeView.currRegion.index          // -1 = none
    property bool addingRegion: latticeView.currRegion.adding

    anchors {
        top: parent.top
        left: parent.left
        right: parent.right
    }
    height: 60

    // event blocker
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
    }

    // background
    Item {
        anchors.fill: parent
        opacity: addingRegion
        layer.enabled: true
        layer.effect: ShaderEffect {
            property color srcColor: Stylesheet.colors[selectedType ? "outputs" : "inputs"][0]
            fragmentShader: "qrc:/shaders/toolbar_grad.frag"
        }

        // TODO: animations :)
//        Behavior on opacity {
//            NumberAnimation {
//                duration: 1000
//                easing.type: Easing.InOutCirc
//            }
//        }
    }

    // content
    RowLayout {
        spacing: 15
        anchors.fill: parent
        anchors {
            leftMargin: 20
            topMargin: 15
            rightMargin: 20
            bottomMargin: 15
        }

        // -- column 1
        Item {
            Layout.preferredWidth: 50
            Layout.preferredHeight: 20

            // prev button
            // visible when a region is selected and > 1
            RegionNavButton {
                type: RegionNavButton.Type.Prev
                visible: !addingRegion

                index: selectedIndex
                maxIndex: selectedType < 0 ? 0 : generatorModel.at(generatorIndex)[selectedType ? "outputCount" : "inputCount"]

                onClicked: latticeView.switchSelectedRegion(selectedType, selectedIndex - 1)
            }

            // "new" label
            // visible when a region is about to be added (ie. drawn on the board)
            Label {
                anchors.fill: parent
                visible: selectedIndex >= 0 && addingRegion

                text: "New"
                font: Stylesheet.fonts.label
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                background: Rectangle {
                    color: Stylesheet.colors[selectedType ? "outputs" : "inputs"][0]
                }
            }
        }

        // delimiter
        LineJoint {
            vertical: true
            opacity: 0.5
            Layout.alignment: Qt.AlignVCenter
        }

        // -- column 2
        RowLayout {
            spacing: 6
            Layout.alignment: Qt.AlignVCenter

            // region / output / input label
            Label {
                font: Stylesheet.fonts.label
                text: selectedType < 0 ? "Region" : (selectedType > 0 ? "Output" : "Input")
                opacity: selectedType < 0 ? 0.5 : 1
                Layout.preferredWidth: 50
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignVCenter
            }

            // line joint
            LineJoint {
                opacity: selectedIndex >= 0
                Layout.alignment: Qt.AlignVCenter
            }

            // counter image
            Image {
                opacity: selectedIndex >= 0

                source: selectedType > 0 ? "qrc:/assets/images/output_counter.svg" : "qrc:/assets/images/input_counter.svg"
                height: 25
                Layout.alignment: Qt.AlignVCenter

                // counter text
                Label {
                    font {
                        family: "Archivo"
                        weight: Font.Medium
                        pixelSize: 16
                    }
                    color: Stylesheet.colors[selectedType > 0 ? "white" : "black"]
                    width: selectedType > 0 ? 21 : 25
                    height: parent.height
                    anchors.left: parent.left
                    text: selectedIndex + 1
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        // -- column fill
        Item { Layout.fillWidth: true }

        // -- column 3
        Item {
            Layout.preferredWidth: contentPusher.width
            Layout.preferredHeight: contentPusher.height

            // visible when >> region selected
            RowLayout {
                id: contentPusher
                spacing: 15
                visible: !addingRegion

                RowLayout {
                    spacing: 6

                    // "Add new" label
                    Label {
                        visible: selectedIndex < 0
                        text: "Add new"
                        font: Stylesheet.fonts.label
                        Layout.alignment: Qt.AlignVCenter
                    }

                    // line joint
                    LineJoint {
                        visible: selectedIndex < 0
                        Layout.alignment: Qt.AlignVCenter
                    }

                    // add new/delete buttons
                    RowLayout {
                        spacing: 10

                        // delete
                        IOEditButton {
                            visible: selectedIndex >= 0
                            editType: 0
                            onClicked: deleteCurrentRegion()
                        }
                        // add
                        IOEditButton {
                            visible: selectedIndex >= 0
                            editType: 1
                            onClicked: currRegion.type ? addNewOutput() : addNewInput()
                        }

                        // add input
                        IOButton {
                            visible: selectedIndex < 0
                            type: 0
                            onClicked: addNewInput()
                        }

                        // add output
                        IOButton {
                            visible: selectedIndex < 0
                            type: 1
                            onClicked: addNewOutput()
                        }
                    }
                }

                // line joint
                LineJoint {
                    vertical: true
                    opacity: 0.5
                    Layout.alignment: Qt.AlignVCenter
                }

                // next region button
                RegionNavButton {
                    type: RegionNavButton.Type.Next
                    visible: !addingRegion

                    index: selectedIndex + 2
                    maxIndex: selectedType < 0 ? 0 : generatorModel.at(generatorIndex)[selectedType ? "outputCount" : "inputCount"]

                    onClicked: switchSelectedRegion(selectedType, selectedIndex + 1)
                }
            }

            // visible when >> adding region
            RowLayout {
                spacing: 15
                anchors.right: parent.right
                visible: selectedIndex >= 0 && addingRegion

                // help label
                Label {
                    font {
                        family: "Archivo"
                        pixelSize: 12
                    }
                    Layout.alignment: Qt.AlignVCenter

                    text: "Draw an area on the lattice to create a new region."
                }

                // cancel add button
                GenericButton {
                    text: "Cancel"
                    activeColor: Stylesheet.colors.white
                    activeTextColor: Stylesheet.colors.black

                    onClicked: switchSelectedRegion(-1, -1)
                }
            }
        }
    }
}
