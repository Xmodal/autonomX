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
                controlColor: Stylesheet.colors.input
            }

            TextField {
                labelText: "Address"
                deactivated: !enableOscIn.on

                defaultText: generatorModel.at(genID).oscInputAddress
                onValueChanged: generatorModel.at(genID).oscInputAddress = newValue
            }
            NumberField {
                labelText: "Port"
                deactivated: !enableOscIn.on

                defaultNum: generatorModel.at(genID).oscInputPort
                onValueChanged: generatorModel.at(genID).oscInputPort = newValue
            }
        }

        RowLayout {
            spacing: Stylesheet.field.spacing
            Layout.fillWidth: true

            CheckField {
                id: enableOscOut
                on: true
                controlColor: Stylesheet.colors.output
            }

            TextField {
                labelText: "Address"
                deactivated: !enableOscOut.on

                defaultText: generatorModel.at(genID).oscOutputAddressTarget
                onValueChanged: generatorModel.at(genID).oscOutputAddressTarget = newValue
            }
            TextField {
                labelText: "Host"
                deactivated: !enableOscOut.on
                // TODO: input masking to only allow a standard IP address format

                defaultText: generatorModel.at(genID).oscOutputAddressHost
                onValueChanged: generatorModel.at(genID).oscOutputAddressHost = newValue
            }
            NumberField {
                labelText: "Port"
                deactivated: !enableOscOut.on

                defaultNum: generatorModel.at(genID).oscOutputPort
                onValueChanged: generatorModel.at(genID).oscOutputPort = newValue
            }
        }
    }
}
