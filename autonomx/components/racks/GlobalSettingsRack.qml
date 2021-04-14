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
//        visible: false

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

//            AreaField {
//                labelText: "User notes"
//                placeholder: "add anything relevant to your setup here!"
//                propName: "userNotes"
//            }
        }
    }

    SubRack {
        subRackTitle: "Full documentation"
//        visible: false

        Label {
            Layout.fillWidth: true
            leftPadding: 30
            rightPadding: 50
            bottomPadding: 30
            topPadding: 20

            text: helpFiles["help_snn.html"]
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            font: Stylesheet.fonts.text
            lineHeight: 1.15

            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
}
