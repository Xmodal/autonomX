// === RACK VIEW ===
// this is where the generator parameters can be modified.
// parameters are stacked in "racks", each determined by their
// respective function. in complement to the essential "GEN" and "PARAMS" racks,
// additional racks can be added for further control and output management.

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

StackLayout {
    id: stackLayout0
    Layout.fillWidth: true
    Layout.fillHeight: true

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: 0

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 0

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true

                RowLayout {
                    Layout.alignment: Qt.AlignRight
                    Layout.fillWidth: false
                    Layout.fillHeight: false

                    Button {
                        text: "Click Me"
                    }
                    Button {
                        text: "Click Me Too"
                    }
                }
            }
        }
    }

    // OSC debug layout:
    ColumnLayout {
        RowLayout {
            SpinBox {
                id: someInt
                value: 2
            }
            Slider {
                id: someDouble
                value: 3.14159
                from: 0.0
                to: 5.0
            }
            TextField {
                id: someText
                text: "hello"
            }
        }

        Button {
            text: "Send OSC"
            font.strikeout: false
            font.underline: false
            font.bold: false
            font.family: "Avenir Next Cyr Medium"
            checked: false
            checkable: false
            onClicked: {
                oscSender.send("/hello", [someInt.value, someDouble.value, someText.text]);
            }
        }

        RowLayout {
            Label {
                text: "Received:"
            }
            Label {
                text: lastMessageReceived
            }
        }
    }
}
