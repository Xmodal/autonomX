import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Item {
    id: header

    Layout.fillWidth: true
    Layout.preferredHeight: 45

    // background
    Rectangle {
        anchors.fill: parent
        color: genID < 0 ? Stylesheet.colors.white : Stylesheet.colors.generators[genID % Stylesheet.colors.generators.length]
    }

    // back arrow
    Image {
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter

        source: "qrc:/assets/images/left-arrow.svg"

        MouseArea {
            anchors.fill: parent
            onClicked: router.currentIndex = 0
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
        color: Stylesheet.colors.darkGrey
    }

    // Middle separator
    Rectangle {
        anchors.bottom: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 12

        width: 1; height: parent.height - anchors.topMargin
        color: Stylesheet.colors.darkGrey
    }

    // Generator name label
    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 20

        text: genID < 0 ? "<no generator selected>" : generatorModel.at(genID).name
        color: Stylesheet.colors.darkGrey
    }
}
