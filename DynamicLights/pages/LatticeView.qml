import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

RowLayout {
    id: latticeView

    Layout.fillWidth: true
    Layout.fillHeight: true

    ShaderEffect {
        width: 400; height: 400
        property int imgWidth: neuronGrid.width
        property int imgHeight: neuronGrid.height
        property variant textureMap: neuronGrid
        vertexShader: "qrc:/shaders/neuron_matrix.vert"
        fragmentShader: "qrc:/shaders/neuron_matrix.frag"
    }

    Image {
        id: neuronGrid
        visible: false
        source: "qrc:/assets/images/neurongrid_20x20.png"
    }
}
