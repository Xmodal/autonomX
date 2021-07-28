import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"

import ca.hexagram.xmodal.autonomx 1.0

ColumnLayout {
    id: paramsRack
    Layout.fillWidth: true
    spacing: 0

    signal changeContent()

    // general
    SubRack {
        id: generalSubRack
        subRackTitle: "General"

        fields: [
            NumberField {
                labelText: "Width"
                propName: "latticeWidth"
                min: 1
                max: 1000
            },

            NumberField {
                labelText: "Height"
                propName: "latticeHeight"
                min: 1
                max: 1000
            },

            SliderField {
                labelText: "Speed"
                propName: "speed"
                minVal: 0
                maxVal: 100
                precision: 0
                unit: "%"
            }
        ]
    }

    Repeater {
        model: metaModel ? Object.keys(metaModel.fieldTree) : []

        SubRack {
            id: paramsSubRack

            subRackTitle: modelData.slice(3)
            Component.onCompleted: {
                regenerateGUI();
                subRackTitleChanged.connect(regenerateGUI)
            }

            Component.onDestruction: destroyGUI()

            // this function is/should be executed everytime the generator type changes
            function regenerateGUI() {
                // disable animations (parent)
                paramsRack.changeContent();
                // disable animations (child)
                paramsSubRack.disableAnimation = true;

                // destroy previous fields
                destroyGUI();

                // define props
                let fieldData = metaModel.fieldTree[modelData];
                let fields = [];

                // loop thru fields of sub-rack
                for (let j = 0; j < fieldData.length; j++) {
                    let field = fieldData[j];

                    // initial props
                    let props = {
                        propName: field.propName,
                        labelText: field.label
                    };
                    let componentToCreate;

                    // flag property
                    if (field.flag) {
                        props.flagName = "flag_" + field.propName;
                    }

                    // determine field type
                    if (field.type === "number") {
                        props.min = field.min;
                        props.max = field.max;

                        componentToCreate = components.NumberField;

                    } else if (field.type === "slider") {
                        props.minVal = field.min;
                        props.maxVal = field.max;
                        props.updateLag = field.updateLag;

                        componentToCreate = components.SliderField;

                    } else if (field.type === "select") {
                        // retrieve choices list
                        props.options = metaModel.enumLabels[field.enumName];

                        componentToCreate = components.SelectField;

                    } else {
                        componentToCreate = components.TextField;
                    }

                    // create object
                    let fieldObj = componentToCreate.createObject(null, props);

                    // push to field array
                    fields.push(fieldObj)
                }

                paramsSubRack.fields = fields;

                // enable animations (child)
            }

            function destroyGUI() {
                for (let i = 0; i < paramsSubRack.fields.length; i++) {
                    if (!this.fields[i]) continue;
                    this.fields[i].reset();
                    this.fields[i].destroy();
                }
            }
        }
    }
}
