import QtQuick 2.9
import QtQuick.Controls 2.3

import "qrc:/stylesheet"

Button {
    id: generatorButton

    // props
    property color backgroundColor
    property string iconSource
    property bool collapsed: false

    font: Stylesheet.fonts.label
    leftPadding: 15
    clip: true

    enabled: state !== "hidden"

    contentItem: Text {
        text: generatorButton.text
        font: generatorButton.font
        color: Stylesheet.colors.white
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    indicator: Image {
        source: iconSource
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.verticalCenter: parent.verticalCenter
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        color: Stylesheet.colors.black

        Rectangle {
            anchors.fill: parent
            color: backgroundColor
            opacity: hovered && !pressed ? 1 : 0.5

            NumberAnimation on opacity {
                duration: 150
                easing.type: Easing.Linear
            }
        }
    }

    states: State {
        name: "hidden"

        PropertyChanges { target: indicator; opacity: 0 }
        PropertyChanges { target: contentItem; opacity: 0 }
        PropertyChanges { target: generatorButton; implicitWidth: 0 }
    }

    transitions: Transition {
        NumberAnimation {
            target: generatorButton
            property: "implicitWidth"
            duration: 500
            easing.type: Easing.OutCubic
        }

        NumberAnimation {
            targets: [indicator, contentItem]
            property: "opacity"
            duration: 150
            easing.type: Easing.Linear
        }
    }
}
