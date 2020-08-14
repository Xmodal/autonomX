import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import "qrc:/stylesheet"
import "../components/ui"

// Main two-column layout
RowLayout {
    id: generatorView
    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 0

    // List of generators
    Item {
        Layout.preferredWidth: parent.width * 0.25
        Layout.minimumWidth: 200
        Layout.maximumWidth: 400
        Layout.fillHeight: true

        Rectangle {
            color: Stylesheet.colors.black
            anchors.fill: parent
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
                //boundsBehavior: Flickable.StopAtBounds
                highlightRangeMode: ListView.ApplyRange
                preferredHighlightBegin: 40
                preferredHighlightEnd: height - 40
                ScrollBar.vertical: ScrollBar {
                    interactive: false
                }

                model: generatorModel
                delegate: GeneratorWidget {
                    onClicked: {
                        window.activeGeneratorIndex = selected ? -1 : model.index
                    }
                }

                // deselect when clicking in empty zone of ListView
                MouseArea {
                    anchors.fill: parent
                    onClicked: window.activeGeneratorIndex = -1
                    z: -1
                }
            }

            // add/delete generator buttons
            RowLayout {
                Layout.alignment: Qt.AlignBottom
                Layout.fillWidth: true
                spacing: 0

                GeneratorButton {
                    id: addGenerator
                    text: "Add"

                    Layout.fillWidth: true
                    implicitWidth: parent.width * 0.5

                    backgroundColor: Stylesheet.colors.generator
                    iconSource: "qrc:/assets/images/add-icon.svg"

                    onClicked: window.addGenerator()
                }

                GeneratorButton {
                    id: deleteGenerator
                    text: "Delete"

                    implicitWidth: parent.width * 0.5

                    backgroundColor: Stylesheet.colors.cancel
                    iconSource: "qrc:/assets/images/delete-icon.svg"

                    onClicked: window.deleteGenerator()
                    state: window.activeGeneratorIndex < 0 || generatorList.count === 1 ? "hidden" : ""
                }
            }
        }
    }

    // list of racks for currently selected generator
    Loader {
        id: rackViewLoader
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop

        source: window.activeGeneratorIndex < 0 ? "" : "qrc:/components/racks/RackView.qml"
    }
}
