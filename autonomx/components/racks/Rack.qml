// Global Rack component
// -- is always extended

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import "qrc:/stylesheet"
import "../fields"

Item {
    id: rack

    // metadata
    property string rackName: "Rack"
    property int generatorIndex: window.activeGeneratorIndex // -1: no assigned generator ID
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
            Layout.preferredHeight: 35

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

            // rack label
            Label {
                id: rackTitle
                text: rackName

                anchors.verticalCenter: parent.verticalCenter
                leftPadding: 20

                font: Stylesheet.fonts.rackLabel
            }

            // collapse rack button
            Button {
                id: btnCollapse

                // dimensions and alignments
                anchors.right: parent.right
                width: 35
                height: 35

                // background
                background: Rectangle {
                    id: btnCollapseBg
                    color: Stylesheet.colors.white
                    opacity: 0

                    states: [
                        State {
                            name: "pressed"; when: btnCollapse.pressed
                            PropertyChanges { target: btnCollapseBg; opacity: 1; color: Stylesheet.colors.generator }
                        },

                        State {
                            name: "hovered"; when: btnCollapse.hovered
                            PropertyChanges { target: btnCollapseBg; opacity: 0.15 }
                        }
                    ]

                    // i don't really like how the NumberAnimation component is essentially doubled
                    // but whatever this works for now i think

                    // TODO: if i ever find a nicer way to implement this
                    // make sure i also apply it to the SliderField handle anim. management
                    transitions: [
                        Transition {
                            from: ""; to: "hovered"
                            NumberAnimation { target: btnCollapseBg; properties: "opacity"; duration: 250; easing.type: Easing.InOutQuad }
                        },

                        Transition {
                            from: "hovered"; to: ""
                            NumberAnimation { target: btnCollapseBg; properties: "opacity"; duration: 250; easing.type: Easing.InOutQuad }
                        }
                    ]
                }

                // expand / collapse icon
                // NICE TO HAVE: icon becomes dark grey on pressed state
                // (couldn't find a good workaround for this unfortunately)
                Image {
                    id: icon
                    source: collapsed ? "qrc:/assets/images/icon-expand.svg" : "qrc:/assets/images/icon-collapse.svg"
                    smooth: true
                    mipmap: false
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }

                // internal property management
                onClicked: collapsed = !collapsed
            }
        }

        // content (extended)
        Loader {
            id: contentLoader
            sourceComponent: content

            clip: true

            Layout.margins: Stylesheet.field.spacing
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
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


