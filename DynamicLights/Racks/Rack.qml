// Global Rack component
// -- is always extended

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import "../Style"
import "../Fields"

Item {
    // metadata
    property string rackName: "Rack"
    property int genID: -1 // -1: no assigned generator ID
    // state flags
    property bool collapsed: false
    property bool removable: true
    // content component
    property Component content

    Layout.fillWidth: true
    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    Layout.preferredHeight: rack.implicitHeight

    ColumnLayout {
        id: rack

        width: parent.width
        spacing: 0

        // top label
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 35

            // background
            Rectangle {
                anchors.fill: parent
                color: "#717171"
            }

            // rack label
            Label {
                id: rackTitle
                text: rackName

                anchors.verticalCenter: parent.verticalCenter
                leftPadding: 20

                font {
                    family: Stylesheet.fonts.sub
                    pixelSize: 16
                    letterSpacing: 0.8
                    capitalization: Font.Capitalize
                }
            }

            // TODO: remove rack button

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
                            PropertyChanges { target: btnCollapseBg; opacity: 1; color: Stylesheet.colors.outputs[genID % Stylesheet.colors.outputs.length] }
                        },

                        State {
                            name: "hovered"; when: btnCollapse.hovered
                            PropertyChanges { target: btnCollapseBg; opacity: 0.3 }
                        }
                    ]

                    // i don't really like how the NumberAnimation component is essentially doubled
                    // but whatever this works for now i think
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
                Image {
                    id: icon
                    source: collapsed ? "qrc:/assets/images/icon-expand.png" : "qrc:/assets/images/icon-collapse.png"
                    smooth: false
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }

                // color inversion effect on button press
                // might be unnecessary to load an entire Qt submodule for this
                // TO-INVESTIGATE: find better way to switch colors on SVG image in this framework
                ColorOverlay {
                    anchors.fill: icon
                    source: icon
                    color: Stylesheet.colors.darkGrey
                    opacity: btnCollapse.pressed ? 1 : 0
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


