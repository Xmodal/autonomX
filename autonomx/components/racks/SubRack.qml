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

    // this was for when we thought toggling OSC on/off would be best done in the rack label thru a checkbox
    // i don't think we really need it anymore though
    //property alias activated: subRackFlag.checked
    property bool activated: true
    property bool collapsed: false
    property bool disableAnimation: false

    property string headerTitle
    property string headerDesc

    Layout.fillWidth: true
    spacing: 0

    // to prevent bouncing on initial load
    Component.onCompleted: heightAnim.duration = Qt.binding(function() { return disableAnimation ? 0 : 500 })

    // top label
    Item {
        Layout.fillWidth: true
        Layout.preferredHeight: 30
        visible: titleBarVisible

        // background
        Rectangle {
            anchors.fill: parent
            color: Stylesheet.colors.black
        }

        RowLayout {
            anchors.fill: parent
            spacing: 0

            // text
            Label {
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                leftPadding: 20
                verticalAlignment: Text.AlignVCenter
                font: Stylesheet.fonts.label
                text: subRackTitle
            }

            // collapse button
            IconButton {
                id: btnCollapse
                size: 30
                iconSource: collapsed ? "qrc:/assets/images/icon-expand.svg" : "qrc:/assets/images/icon-collapse.svg"
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                onClicked: {
                    // this will prevent height from being animated in parent rack
                    // (see Loader comments for more behavior info)
                    disableAnimation = false;
                    subRack.parent.parent.disableAnimation = true
                    collapsed = !collapsed
                }
            }
        }
    }

    // sub rack content
    Rectangle {
        visible: titleBarVisible
        Layout.fillWidth: true
        Layout.preferredHeight: collapsed ? 0 : childrenRect.height + 50
        clip: true
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

        Behavior on Layout.preferredHeight {
            NumberAnimation {
                id: heightAnim
                duration: 0
                easing.type: Easing.OutCubic
            }
        }
    }
}
