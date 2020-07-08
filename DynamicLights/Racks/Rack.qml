// Global Rack component
// -- is always extended

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

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
        RowLayout {
            Layout.fillWidth: true
            spacing: 0

            Label {
                id: rackTitle
                text: rackName

                leftPadding: 20
                bottomPadding: 10
                topPadding: 10
                Layout.fillWidth: true

                font {
                    family: Stylesheet.fonts.sub
                    pixelSize: 16
                    letterSpacing: 0.8
                    capitalization: Font.Capitalize
                }

                background: Rectangle {
                    color: "#717171"
                }
            }

            // TODO: remove rack button

            // collapse rack button
            Button {
                id: btnCollapse

                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: 35
                Layout.fillHeight: true

                background: Rectangle {
                    color: "#4A4A4A"
                }

                Image {
                    id: icon
                    source: collapsed ? "qrc:/assets/images/icon-expand.svg" : "qrc:/assets/images/icon-collapse.svg"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }

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


