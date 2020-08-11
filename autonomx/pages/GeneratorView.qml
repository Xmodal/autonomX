import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import "qrc:/stylesheet"
import "qrc:/components/delegates"

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

        ListView {
            anchors.fill: parent
            orientation: Qt.Vertical
            model: generatorModel
            delegate: GeneratorWidget {
                onClicked: window.activeGeneratorIndex = model.index
            }
        }

        // TODO: new generator button here
        RowLayout {
            anchors.bottom: parent.bottom
            width: parent.width
            spacing: 0

            GeneratorButton {
                id: addGenerator
                text: "Add"
                iconSource: "qrc:/assets/images/add-icon.svg"
                Layout.fillWidth: true
                backgroundColor: Stylesheet.colors.generator
            }

            GeneratorButton {
                id: deleteGenerator
                text: "Delete"
                Layout.fillWidth: true
                backgroundColor: Stylesheet.colors.cancel
                iconSource: "qrc:/assets/images/delete-icon.svg"
            }
        }
    }

    // list of racks for currently selected generator
    Loader {
        id: rackViewLoader
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop

        property int genID: window.activeGeneratorIndex

        source: genID < 0 ? "" : "qrc:/components/racks/RackView.qml"
    }
}
