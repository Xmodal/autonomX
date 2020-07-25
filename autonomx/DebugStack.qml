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
}
