// Global Rack component
// -- is always extended

import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"
import "../ui"

Item {
    id: rack

    // metadata
    property string rackName: "Rack"

    // state flags
    property bool collapsed: false
    property bool removable: false
    // content component
    property alias content: contentLoader.sourceComponent

    Layout.fillWidth: true
    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    Layout.preferredHeight: container.implicitHeight

    // background
    Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors.darkGrey
    }

    MouseArea {
        anchors.fill: parent
        onClicked: forceActiveFocus()
    }

    ColumnLayout {
        id: container

        width: parent.width
        spacing: 0

        // top label
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            // background
            Rectangle {
                anchors.fill: parent
                color: Stylesheet.colors.generator

                layer.enabled: true
                layer.effect: ShaderEffect {
                    property color overlayColor: Stylesheet.colors.darkGrey
                    property real minAlpha: 0.95
                    property real maxAlpha: 0.75
                    property bool vertical: false
                    fragmentShader: "qrc:/shaders/gradient_overlay.frag"
                }
            }

            RowLayout {
                anchors.fill: parent
                spacing: 0

                // rack label
                Label {
                    id: rackTitle
                    text: rackName

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                    Layout.leftMargin: 20

                    font: Stylesheet.fonts.rackLabel
                }

                // collapse rack button
                IconButton {
                    id: btnCollapse
                    iconSource: collapsed ? "qrc:/assets/images/icon-expand.svg" : "qrc:/assets/images/icon-collapse.svg"
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                    onClicked: {
                        // re-enable animation
                        contentLoader.disableAnimation = false
                        collapsed = !collapsed
                    }
                }
            }
        }

        // content (extended)
        Loader {
            id: contentLoader
            clip: true

            // when true: below behavior is disabled
            // - is set to true when any SubRack instance is collapsed/expanded
            // - is set to false when the contained Rack is collapsed/expanded
            // TODO: cross-animation blocker to prevent weird bugs! :) depending on how severe the bugs actually are (untested)
            property bool disableAnimation: true

            Layout.fillWidth: true
            Layout.preferredHeight: collapsed ? 0 : implicitHeight

            // animation management
            Behavior on Layout.preferredHeight {
                NumberAnimation {
                    duration: contentLoader.disableAnimation ? 0 : 500
                    easing.type: Easing.OutCubic
                }
            }
        }

        // disable animations when new content is being loaded
        // (called inside each discrete loaded component when content height is likely to change)
        Connections {
            target: contentLoader.item
            function onChangeContent() {
                contentLoader.disableAnimation = true
            }
        }
    }

    // bottom border
    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: Stylesheet.colors.black
        opacity: 0.3
    }
}
