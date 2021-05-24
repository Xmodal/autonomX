import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../components/ui"

Item {
    property bool choicesOpen: false

    Layout.preferredWidth: 320
    Layout.fillHeight: true

    // background
    Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors.darkGrey
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ListView {
            id: generatorList

            property int prevCount: 0

            Layout.fillWidth: true
            Layout.fillHeight: true

            orientation: Qt.Vertical
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {
                interactive: false
            }

            model: generatorModel
            delegate: GeneratorWidget {
                onClicked: {
                    choicesOpen = false
                    window.activeGeneratorIndex = selected ? -1 : model.index
                }
            }

            // deselect when clicking in empty zone of ListView
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    choicesOpen = false
                    window.activeGeneratorIndex = -1
                }

                z: choicesOpen ? 1 : -1
            }

            // masking overlay
            Rectangle {
                id: overlay
                anchors.fill: parent
                color: Stylesheet.colors.black
                opacity: choicesOpen ? 0.6 : 0

                Behavior on opacity {
                    NumberAnimation {
                        duration: 250
                    }
                }
            }
        }

        // add/delete generator buttons
        ColumnLayout {
            id: generatorAddCtn

            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true
            spacing: 0
            Layout.bottomMargin: choicesOpen ? 0 : -childrenRect.height + addGenerator.height
            z: 10

            Behavior on Layout.bottomMargin {
                NumberAnimation {
                    duration: 400
                    easing.type: Easing.OutCubic
                }
            }

            GeneratorButton {
                id: addGenerator
                text: "Add"

                Layout.fillWidth: true

                backgroundColor: Stylesheet.colors.generator
                iconSource: "qrc:/assets/images/plus.svg"

                onClicked: choicesOpen = !choicesOpen
            }

            ListView {
                Layout.fillWidth: true
                Layout.preferredHeight: childrenRect.height
                Layout.maximumHeight: 140
                z: -1

                snapMode: ListView.SnapToItem
                boundsBehavior: Flickable.StopAtBounds

                model: generatorMetaModel

                delegate: Button {
                    id: genTypeChoice

                    property string type: model.type
                    text: model.name

                    width: parent.width
                    leftPadding: hovered ? 25 : 15

                    contentItem: Label {
                        font: parent.font
                        text: parent.text
                        verticalAlignment: Text.AlignVCenter
                        opacity: hovered ? 1 : 0.5

                        Behavior on opacity {
                            NumberAnimation {
                                duration: 200
                                easing.type: Easing.Linear
                            }
                        }
                    }

                    background: Rectangle {
                        implicitHeight: 40
                        color: Stylesheet.colors.black

                        Rectangle {
                            implicitHeight: 40
                            implicitWidth: parent.width
                            color: Stylesheet.colors.darkGrey
                            x: hovered ? 0 : -width
                            opacity: genTypeChoice.pressed ? 0 : 1

                            Behavior on x {
                                NumberAnimation {
                                    duration: 400
                                    easing.type: Easing.OutCubic
                                }
                            }
                        }
                    }

                    font: Stylesheet.fonts.text

                    onClicked: {
                        choicesOpen = false
                        window.addGenerator(type)
                    }

                    Behavior on leftPadding {
                        NumberAnimation {
                            duration: 400
                            easing.type: Easing.OutCubic
                        }
                    }

                    CursorShaper {}
                }
            }
        }
    }
}
