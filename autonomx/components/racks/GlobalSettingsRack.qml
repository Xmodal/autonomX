import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"

ColumnLayout {
    Layout.fillWidth: true
    spacing: 0

    SubRack {
        titleBarVisible: false

        ColumnLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 30
            Layout.rightMargin: 30
            Layout.topMargin: 20
            Layout.bottomMargin: 30
            spacing: 30

            ColumnLayout {
                spacing: 10

                Label {
                    id: generatorName
                    text: "Spiking Neural Network"
                    font: Stylesheet.fonts.title

                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 1
                        color: parent.color
                    }
                }

                Label {
                    id: generatorDescription
                    text: "An interconnected network of biologically-modeled neurons."
                    font: Stylesheet.fonts.text
                }
            }

            TextField {
                fieldWidth: -1
                Layout.fillWidth: true
                labelText: "User notes"
                placeholder: "add anything relevant to your setup here!"
                propName: "userNotes"
            }

            // TODO: turn this into a proper text area!
//            AreaField {
//                labelText: "User notes"
//                placeholder: "add anything relevant to your setup here!"
//                propName: "userNotes"
//            }
        }
    }

    // documentation rack (collapsed by default)
    SubRack {
        subRackTitle: "SNN overview"
        collapsed: true

        fields: [
            Label {
                width: parent.width

                text: helpFiles["help_snn_desc.html"]
                textFormat: Text.RichText
                wrapMode: Text.WordWrap
                font: Stylesheet.fonts.text
                lineHeight: 1.15

                onLinkActivated: Qt.openUrlExternally(link)
            }
        ]
    }

    SubRack {
        subRackTitle: "Parameter help"
        collapsed: true

        fields: [
            Label {
                width: parent.width

                text: helpFiles["help_snn_params.html"]
                textFormat: Text.RichText
                wrapMode: Text.WordWrap
                font: Stylesheet.fonts.text
                lineHeight: 1.15

                onLinkActivated: Qt.openUrlExternally(link)
            }

        ]
    }
}
