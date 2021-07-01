import QtQuick 2.12
import QtQuick.Controls 2.12

IOButton {
    id: ioEditButton

    type: -1
    text: ""
    property int editType: 0        // remove = 0; add = 1

    Image {
        source: "qrc:/assets/images/" + (editType ? 'plus' : 'minus') + '.svg'
        width: 9
        height: 9
        fillMode: Image.PreserveAspectFit

        layer.enabled: true
        layer.effect: ShaderEffect {
            property bool checked: ioEditButton.pressed
            fragmentShader: "qrc:/shaders/icon_button.frag"
        }

        anchors.centerIn: parent
    }
}
