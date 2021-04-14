import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"
import "../ui"

ColumnLayout {
    id: subRack

    property string subRackTitle
    property bool titleBarVisible: true
    property list<Item> fields
    property int columnCount: 3

    property bool flagActive: false
    property color flagColor: Stylesheet.colors.white
    //property string switchProp
    property alias activated: subRackFlag.checked

    property string headerTitle
    property string headerDesc

    Layout.fillWidth: true
    spacing: 0

    Label {
        visible: titleBarVisible
        Layout.fillWidth: true
        Layout.preferredHeight: 30
        leftPadding: 20
        verticalAlignment: Text.AlignVCenter
        font: Stylesheet.fonts.label
        text: subRackTitle

        background: Rectangle {
            color: Stylesheet.colors.black
        }

        CheckBox {
            id: subRackFlag
            checked: true
            visible: flagActive
            controlColor: flagColor
            size: 16
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
        }
    }

    Rectangle {
        visible: titleBarVisible
        Layout.fillWidth: true
        implicitHeight: childrenRect.height ? childrenRect.height + 50 : 0
        color: Stylesheet.setAlpha(Stylesheet.colors.darkGrey, 0.6)

        Grid {
            enabled: activated
            opacity: enabled ? 1 : 0.4

            Behavior on opacity {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top

                leftMargin: 30
                rightMargin: 30
                topMargin: 20
            }
            columns: columnCount
            columnSpacing: Stylesheet.field.spacing
            rowSpacing: Stylesheet.field.spacing * 1.5

            // fields are directly set as children of Flow object here
            children: fields
        }
    }
}
