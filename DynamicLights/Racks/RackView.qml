// === RACK VIEW ===
// this is where the generator parameters can be modified.
// parameters are stacked in "racks", each determined by their
// respective function. in complement to the essential "GEN" and "PARAMS" racks,
// additional racks can be added for further control and output management.


import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0

import "../"

ColumnLayout {
    id: rackView

    property int genID: item.genID

    Layout.fillWidth: true
    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
    spacing: 0

    GenRack { genID: rackView.genID; }
    ParamsRack { genID: rackView.genID; }
}
