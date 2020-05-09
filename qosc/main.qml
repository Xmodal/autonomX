import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Window {
    title: qsTr("OSC")
    width: 720
    height: 480
    visible: true

    property string lastMessageReceived: ""

    function handleMessageReceived(oscPath, oscArguments) {
        console.log("QML-Received OSC: " + oscPath + " " + oscArguments);
        lastMessageReceived = oscPath + " " + oscArguments;
    }

    Connections {
        target: oscReceiver

        onMessageReceived: {
            handleMessageReceived(oscAddress, message);
        }
    }

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
