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

    // list of racks for currently selected generator
    Loader {
        id: rackViewLoader
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop

        source: window.activeGeneratorIndex < 0 ? "" : "qrc:/components/racks/RackView.qml"
    }
}
