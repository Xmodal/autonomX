pragma Singleton

import QtQuick 2.11

// NICE TO HAVE: separate singletons for colors/fonts/components/etc
QtObject {
    // COLORS
    property QtObject colors: QtObject {
        readonly property color white: "#ECECEC"
        readonly property color black: "#151515"
        readonly property color darkGrey: "#212121"

        readonly property color generator: "#437C61"
        readonly property color cancel: "#BF3434"

        readonly property variant inputs:  [ "#3BB3E7", "#3B9FE7", "#3582F5", "#3B4CE7" ]
        readonly property variant outputs: [ "#C53412", "#D65B16", "#E7961D", "#EEBE16" ]
        readonly property int variations: 4
    }

    // FONTS
    property QtObject fonts: QtObject {
        readonly property font label: Qt.font({ family: "Archivo", pixelSize: 12, weight: Font.Medium, letterSpacing: 12 * 0.03, capitalization: Font.AllUppercase })
        readonly property font rackLabel: Qt.font({ family: "Questrial", pixelSize: 16, letterSpacing: 16 * 0.05, capitalization: Font.AllUppercase })
        readonly property font text: Qt.font({ family: "Archivo", pixelSize: 14, weight: Font.Regular })
        readonly property font fieldText: Qt.font({ family: "Archivo", pixelSize: 16, weight: Font.Medium, letterSpacing: 16 * 0.01 })
        readonly property font title: Qt.font({ family: "Archivo", pixelSize: 20, weight: Font.Medium, letterSpacing: 20 * 0.01 })
    }

    // SPACINGS
    property QtObject field: QtObject {
        readonly property int padding: 8
        readonly property int initialWidth: 150
        readonly property int spacing: 20
    }

    // OTHER VALUES
    property real headerHeight: 40

    // TOOLS
    function setAlpha(color, alpha) {
        return Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, alpha);
    }
}
