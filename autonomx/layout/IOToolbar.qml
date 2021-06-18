import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import "qrc:/stylesheet"
import "../components/ui"

Item {
    id: ioToolbar

    property int selectedType: -1           // -1 = none;   0 = input;  1 = output
    property int selectedIndex: -1          // -1 = none
    property bool addingRegion: false

    anchors {
        top: parent.top
        left: parent.left
        right: parent.right
    }
    height: 60

    // background
    Item {
        anchors.fill: parent
        opacity: addingRegion
        layer.enabled: true
        layer.effect: ShaderEffect {
            property color srcColor: Stylesheet.colors.outputs[0]
            fragmentShader: "qrc:/shaders/toolbar_grad.frag"
        }

        Behavior on opacity {
            NumberAnimation {
                duration: 1000
                easing.type: Easing.InOutCirc
            }
        }
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

            // "region" label
            // visible when no region is selected
            Label {
                anchors.fill: parent
                visible: selectedIndex < 0
                text: "Region"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font: Stylesheet.fonts.label
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
                    color: Stylesheet.colors.outputs[0]
                }
            }

            // prev button
            // visible when a region is selected and > 1
            RegionNavButton {
                type: RegionNavButton.Type.Prev
                visible: selectedIndex >= 0 && !addingRegion
            }
        }

        // delimiter
        Rectangle {
            width: 1
            height: 30
            color: Stylesheet.colors.white
            opacity: 0.5
            Layout.alignment: Qt.AlignVCenter
        }

        // -- column 2
        // always contains input/output label and index shape
        RowLayout {
            spacing: 6

            visible: selectedIndex >= 0 && !addingRegion

            Label {
                font: Stylesheet.fonts.label
                text: selectedType > 0 ? "Output" : "Input"
                Layout.preferredWidth: 50
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignVCenter
            }

            Rectangle {
                width: 30
                height: 1
                color: Stylesheet.colors.white
                Layout.alignment: Qt.AlignVCenter
            }

            Image {
                source: selectedType > 0 ? "qrc:/assets/images/output_counter.svg" : "qrc:/assets/images/input_counter.svg"
                height: 25
                Layout.alignment: Qt.AlignVCenter

                Label {
                    font {
                        family: "Archivo"
                        weight: Font.Medium
                        pixelSize: 16
                    }
                    width: selectedType > 0 ? 21 : 25
                    height: parent.height
                    anchors.left: parent.left
                    text: "5"
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        // -- column fill
        Item { Layout.fillWidth: true }

        Item {
            Layout.preferredWidth: contentPusher.width
            Layout.preferredHeight: contentPusher.height

            // no region selected
            RowLayout {
                spacing: 15
                visible: selectedIndex < 0

                // delimiter
                Rectangle {
                    width: 1
                    height: 30
                    color: Stylesheet.colors.white
                    opacity: 0.5
                    Layout.alignment: Qt.AlignVCenter
                }

                // "add new" label
                Label {
                    text: "Add new"
                    width: 85
                }
            }

            // region selected
            RowLayout {
                id: contentPusher
                spacing: 15

                visible: selectedIndex >= 0 && !addingRegion

                // -- column 3
                // contains next button
                RegionNavButton {
                    type: RegionNavButton.Type.Next
                }

                // delimiter
                Rectangle {
                    width: 1
                    height: 30
                    color: Stylesheet.colors.white
                    opacity: 0.5
                    Layout.alignment: Qt.AlignVCenter
                }

                // -- column 4
                // add new, delete, cancel buttons
                RowLayout {
                    spacing: 5

                    GenericButton {
                        text: "Add new"
                        activeColor: Stylesheet.colors.white
                        activeTextColor: Stylesheet.colors.black

                        onClicked: {

                        }
                    }

                    GenericButton {
                        text: "Delete"
                        activeColor: Stylesheet.colors.white
                        activeTextColor: Stylesheet.colors.black

                        onClicked: {

                        }
                    }
                }
            }

            // region adding
            RowLayout {
                anchors.right: parent.right
                visible: selectedIndex >= 0 && addingRegion

                Label {
                    font {
                        family: "Archivo"
                        pixelSize: 12
                    }
                    Layout.alignment: Qt.AlignVCenter

                    text: "Draw an area on the lattice to create a new region."
                }

                GenericButton {
                    text: "Cancel"
                    activeColor: Stylesheet.colors.white
                    activeTextColor: Stylesheet.colors.black

                    onClicked: {

                    }
                }
            }
        }
    }
}
