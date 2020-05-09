import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

/**
 * Widget to control a generator.
 */
ColumnLayout {
    property int generatorIndex: 1
    property string generatorName: "Generator X"

    RowLayout {
        Layout.fillWidth: true

        Label {
            text: generatorIndex
        }

        Label {
            text: generatorName
            Layout.fillWidth: true
        }
   }
}
