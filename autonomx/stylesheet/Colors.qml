pragma Singleton

import QtQuick 2.11

QtObject {
    readonly property color white: "#ECECEC"
    readonly property color black: "#151515"
    readonly property color darkGrey: "#212121"

    readonly property color generator: "#437C61"
    readonly property color cancel: "#BF3434"

    readonly property variant inputs:  [ "#3BB3E7", "#3B9FE7", "#3582F5", "#3B4CE7" ]
    readonly property variant outputs: [ "#C53412", "#D65B16", "#E7961D", "#EEBE16" ]
    readonly property int variations: 4

    // TOOLS
    function setAlpha(color, alpha) {
        return Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, alpha);
    }
}
