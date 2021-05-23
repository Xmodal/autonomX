import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../components/ui"

Item {
    Layout.preferredWidth: 320
    Layout.fillHeight: true

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
                    generatorAddCtn.choicesOpen = false
                    window.activeGeneratorIndex = selected ? -1 : model.index
                }
            }

            // deselect when clicking in empty zone of ListView
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    generatorAddCtn.choicesOpen = false
                    window.activeGeneratorIndex = -1
                }

                z: -1
            }
        }

        // add/delete generator buttons
        ColumnLayout {
            id: generatorAddCtn

            property bool choicesOpen: false

            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true
            spacing: 0
            Layout.bottomMargin: choicesOpen ? 0 : -childrenRect.height + addGenerator.height

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

                onClicked: generatorAddCtn.choicesOpen = true
            }

            Repeater {
                model: generatorMetaModel

                Button {
                    id: genTypeChoice

                    property string type: model.type
                    text: model.name

                    Layout.fillWidth: true
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
                        generatorAddCtn.choicesOpen = false
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
