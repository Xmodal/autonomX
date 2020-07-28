// === RACK VIEW ===
// this is where the generator parameters can be modified.
// parameters are stacked in "racks", each determined by their
// respective function. in complement to the essential "GEN" and "PARAMS" racks,
// additional racks can be added for further control and output management.

import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0

import "qrc:/stylesheet"

ScrollView {
    id: rackView

    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
    ScrollBar.vertical.interactive: false
    ScrollBar.vertical.hoverEnabled: false

    ColumnLayout {
        width: parent.width
        spacing: 0

        GenRack {}
        ParamsRack {}
        OscRack {}
    }
}
