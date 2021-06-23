import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

RowLayout {
    enum Type { Prev, Next }

    id: regionNavButton
    property int type: RegionNavButton.Type.Prev
    property bool isNext: type === RegionNavButton.Type.Next

    spacing: 10
    layoutDirection: isNext ? Qt.LeftToRight : Qt.RightToLeft

    opacity: enabled

    // button
    GenericButton {
        id: navButton
        height: 20
        paddingSide: 8

        activeColor: Stylesheet.colors.white
        activeTextColor: Stylesheet.colors.black

        contentItem: Image {
            source: "qrc:/assets/images/arrow.svg"
            width: 11

            rotation: isNext ? 0 : 180

            opacity: navButton.hovered ? 1 : 0.7

            layer.enabled: true
            layer.effect: ShaderEffect {
                property bool checked: navButton.pressed
                fragmentShader: "qrc:/shaders/icon_button.frag"
            }
        }
    }

    // index
    Label {
        id: navIndex
        font: Stylesheet.fonts.label
        text: "6"
    }
}
