import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Item {
    id: header

    Layout.fillWidth: true
    Layout.preferredHeight: 40

    // background
    Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors.white
        gradient: Gradient {
            GradientStop { position: 0; color: Stylesheet.setAlpha(Stylesheet.colors.darkGrey, 0.9) }
            GradientStop { position: 1; color: Stylesheet.setAlpha(Stylesheet.colors.darkGrey, 0.8) }
        }
    }

    // "Lattice view" label
    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 20

        text: "Lattice view"
        font {
            family: Stylesheet.fonts.sub
            pixelSize: 13
            letterSpacing: 13 * 0.05
            capitalization: Font.AllUppercase
        }
    }

    // Generator name label
    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 20

        font {
            weight: Font.DemiBold
        }

        text: genID < 0 ? "<no generator selected>" : generatorModel.at(genID).name
    }
}
