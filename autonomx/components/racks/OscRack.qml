import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import "qrc:/stylesheet"
import "../delegates"
import "../fields"

Rack {
    id: oscRack

    rackName: "OSC"

    content: ColumnLayout {
        spacing: Stylesheet.field.spacing

        RowLayout {
            spacing: Stylesheet.field.spacing
            Layout.fillWidth: true

            CheckField {
                id: enableOscIn
                on: true
                controlColor: Stylesheet.colors.inputs[0]
            }

            TextField {
                labelText: "Address"
                deactivated: !enableOscIn.on

                defaultText: generatorModel.at(generatorIndex).oscInputAddress
                onValueChanged: generatorModel.at(generatorIndex).oscInputAddress = newValue
            }
            NumberField {
                labelText: "Port"
                deactivated: !enableOscIn.on

                defaultNum: generatorModel.at(generatorIndex).oscInputPort
                onValueChanged: generatorModel.at(generatorIndex).oscInputPort = newValue
            }
        }

        RowLayout {
            spacing: Stylesheet.field.spacing
            Layout.fillWidth: true

            CheckField {
                id: enableOscOut
                on: true
                controlColor: Stylesheet.colors.outputs[0]
            }

            TextField {
                labelText: "Address"
                deactivated: !enableOscOut.on

                defaultText: generatorModel.at(generatorIndex).oscOutputAddressTarget
                onValueChanged: generatorModel.at(generatorIndex).oscOutputAddressTarget = newValue
            }
            TextField {
                labelText: "Host"
                deactivated: !enableOscOut.on
                // TODO: input masking to only allow a standard IP address format

                defaultText: generatorModel.at(generatorIndex).oscOutputAddressHost
                onValueChanged: generatorModel.at(generatorIndex).oscOutputAddressHost = newValue
            }
            NumberField {
                labelText: "Port"
                deactivated: !enableOscOut.on

                defaultNum: generatorModel.at(generatorIndex).oscOutputPort
                onValueChanged: generatorModel.at(generatorIndex).oscOutputPort = newValue
            }
        }
    }
}
