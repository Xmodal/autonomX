import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "qrc:/stylesheet"
import "../fields"
import "../ui"

Item {
    property alias open: button.checked
    property bool showCpu: false

    Layout.preferredWidth: open ? 360 : 100
    Layout.fillHeight: true

    Button {
        id: button
        checkable: true
        anchors.fill: parent

        background: Rectangle {
            anchors.fill: parent
            color: Stylesheet.colors.darkGrey
        }

        // CPU load indicator
        Rectangle {
            id: cpuLoad
            visible: showCpu
            anchors.fill: parent
            color: Stylesheet.colors.generator
            opacity: 0.25

            transform: Scale {
                xScale: 0.25
            }
        }

        RowLayout {
            anchors.fill: parent

            Label {
                text: "Options"
                opacity: button.hovered || button.checked ? 1 : 0.5
                font {
                    family: "Archivo"
                    capitalization: Font.AllUppercase
                    weight: Font.Medium
                    pixelSize: 11
                }

                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: 10
            }

            Item { Layout.fillWidth: true }

            RowLayout {
                Layout.rightMargin: 10
                spacing: 7

                Label {
                    visible: showCpu

                    font {
                        family: "Archivo"
                        weight: Font.Medium
                        pixelSize: 11
                    }
                    text: "13%"
                    Layout.alignment: Qt.AlignVCenter
                }

                Image {
                    source: "qrc:/assets/images/down-caret.svg"
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }

        CursorShaper {}
    }

    Rectangle {
        anchors.top: button.bottom
        width: parent.width
        height: childrenRect.height
        color: Stylesheet.colors.black
        visible: open

        // evt blocker
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onClicked: mainContent.forceActiveFocus()
        }

        ColumnLayout {
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20

            id: optsContent
            spacing: 20

            // fields
            RowLayout {
                Layout.fillWidth: true
                Layout.topMargin: 15
                spacing: 20

                NumberField {
                    Layout.fillWidth: true
                    labelText: "In port"
                    fieldBg: Stylesheet.colors.darkGrey

                    defaultNum: 7000
                    // target: oscEngine
                    // propName: "receiverPort"
                }

                NumberField {
                    Layout.fillWidth: true
                    labelText: "Out port"
                    defaultNum: 7001
                    fieldBg: Stylesheet.colors.darkGrey
                }
            }
            TextField {
                Layout.fillWidth: true
                fieldWidth: 320
                labelText: "Out host"
                defaultText: "127.0.0.1"
                fieldBg: Stylesheet.colors.darkGrey
            }

            // delimiter
            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Stylesheet.colors.white
                opacity: 0.5
            }

            // application info
            RowLayout {
                opacity: 0.5
                Layout.bottomMargin: 15

                Label {
                    text: "autonomX\nby Xmodal"
                    lineHeight: 1
                    font {
                        family: "Archivo"
                        pixelSize: 10
                    }
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: "version 0.1.0\n22-09-2020"
                    lineHeight: 1
                    horizontalAlignment: Text.AlignRight
                    font {
                        family: "Archivo"
                        pixelSize: 10
                    }
                }
            }
        }
    }
}
