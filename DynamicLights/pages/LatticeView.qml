import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

RowLayout {
    id: latticeView

    Layout.fillWidth: true
    Layout.fillHeight: true

    ShaderEffect {
        width: 400; height: 400
        property int cw: width
        property int ch: height
        property int cols: 20
        property int rows: 20
        property Image textureMap: Image { id: neuronGrid; source: "qrc:/assets/images/neurongrid_20x20.png" }
        fragmentShader: "qrc:/shaders/neuron_matrix.frag"
    }
}
