pragma Singleton

import QtQuick 2.11

// NICE TO HAVE: separate singletons for colors/fonts/components/etc
QtObject {
    // COLORS
    property QtObject colors: QtObject {
        readonly property color white: "#ECECEC"
        readonly property color black: "#151515"
        readonly property color darkGrey: "#212121"
    }

    // FONTS
    property QtObject fonts: QtObject {
        readonly property string main: "Overpass"
        readonly property string sub: "Questrial"
    }

    // SPACINGS
    property QtObject field: QtObject {
        //readonly property int padding_LR: 0
        readonly property int padding_TB: 8
    }

    // TOOLS
    function setAlpha(color, alpha) {
        return Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, alpha);
    }
}
