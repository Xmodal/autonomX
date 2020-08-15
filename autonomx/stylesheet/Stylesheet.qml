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
    }

    // FONTS
    property QtObject fonts: QtObject {
        readonly property font label: Qt.font({ family: "Questrial", pixelSize: 13, letterSpacing: 13 * 0.05, capitalization: Font.AllUppercase })
        readonly property font rackLabel: Qt.font({ family: "Questrial", pixelSize: 16, letterSpacing: 16 * 0.05, capitalization: Font.AllUppercase })
        readonly property font text: Qt.font({ family: "Overpass", pixelSize: 16, weight: Font.DemiBold })
    }

    // SPACINGS
    property QtObject field: QtObject {
        readonly property int padding: 8
        readonly property int initialWidth: 150
        readonly property int spacing: 20
    }

    // TOOLS
    function setAlpha(color, alpha) {
        return Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, alpha);
    }
}
