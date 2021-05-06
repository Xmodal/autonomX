import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"

import ca.hexagram.xmodal.autonomx 1.0

ColumnLayout {
    Layout.fillWidth: true
    spacing: 0

    SubRack {
        subRackTitle: "General"

        fields: [
            NumberField {
                labelText: "Width"
                min: 1
                max: 1000
                propName: "latticeWidth"
            },

            NumberField {
                labelText: "Height"
                min: 1
                max: 1000
                propName: "latticeHeight"
            },

            SliderField {
                labelText: "Time scale"
                propName: "timeScale"
                maxVal: 0.03
            }
        ]
    }

    SubRack {
        subRackTitle: "Rule behavior"

        fields: [
            NumberField {
                labelText: "Rule"
                propName: "rule"
            }
        ]
    }
}
