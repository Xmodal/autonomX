import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/stylesheet"

Label {
    id: errorPopup

    property string title: "Invalid name"
    property string errorText: "This name is already taken."

    // background + error text
    text: errorText
    color: Stylesheet.colors.white
    font: Stylesheet.fonts.text

    background: Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors.cancel
    }

    leftPadding: 15
    rightPadding: 15
    bottomPadding: 5
    topPadding: 17

    // title
    Label {
        anchors.top: parent.top
        anchors.topMargin: -10
        anchors.left: parent.left
        anchors.leftMargin: 10

        text: title
        leftPadding: 10
        rightPadding: 10
        color: Stylesheet.colors.cancel
        font: Stylesheet.fonts.rackLabel

        background: Rectangle {
            anchors.fill: parent
            color: Stylesheet.colors.white
        }
    }
}
