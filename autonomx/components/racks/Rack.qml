// Global Rack component
// -- is always extended

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import "qrc:/stylesheet"
import "../fields"
import "../ui"

Item {
    id: rack

    // metadata
    property string rackName: "Rack"

    // state flags
    property bool collapsed: false
    property bool removable: true
    // content component
    property Component content

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

                    onClicked: collapsed = !collapsed
                }
            }
        }

        // content (extended)
        Loader {
            id: contentLoader
            sourceComponent: content
            clip: true

            property int generatorIndex: window.activeGeneratorIndex // -1: no assigned generator ID

            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight

            // animation management
            states: [
                State {
                    name: "collapsed"; when: collapsed
                    PropertyChanges { target: contentLoader; Layout.preferredHeight: 0; Layout.topMargin: 0; Layout.bottomMargin: 0; }
                }
            ]
            transitions: Transition {
                NumberAnimation {
                    properties: "Layout.preferredHeight,Layout.topMargin,Layout.bottomMargin"
                    duration: 500
                    easing.type: Easing.InOutCubic
                }
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
