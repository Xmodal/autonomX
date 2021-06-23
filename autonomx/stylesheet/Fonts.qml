pragma Singleton

import QtQuick 2.11

QtObject {
    readonly property font label: Qt.font({ family: "Questrial", pixelSize: 13, letterSpacing: 13 * 0.05, capitalization: Font.AllUppercase })
    readonly property font rackLabel: Qt.font({ family: "Questrial", pixelSize: 16, letterSpacing: 16 * 0.05, capitalization: Font.AllUppercase })
    readonly property font text: Qt.font({ family: "Archivo", pixelSize: 14, weight: Font.Regular })
    readonly property font fieldText: Qt.font({ family: "Archivo", pixelSize: 16, weight: Font.Medium, letterSpacing: 16 * 0.01 })
    readonly property font title: Qt.font({ family: "Archivo", pixelSize: 20, weight: Font.Medium, letterSpacing: 20 * 0.01 })
}
