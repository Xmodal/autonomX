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

    signal startedGenerating()
    signal finishedGenerating()

    Repeater {
        model: metaModel ? Object.keys(metaModel.fieldTree) : []

        SubRack {
            id: paramsSubRack

            subRackTitle: modelData.slice(3)
            onSubRackTitleChanged: regenerateGUI()

            function createTimer() {
                return Qt.createQmlObject("import QtQuick 2.9; Timer {}", window);
            }

            function delay(time, cb) {
                let timer = createTimer();
                timer.interval = time;
                timer.repeat = false;
                timer.triggered.connect(cb);
                timer.start();
            }

            // this function is/should be executed everytime the generator type changes
            function regenerateGUI() {
                paramsRack.startedGenerating();

                // reset fields
                paramsSubRack.fields = [];

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
                        props.flagName = "flag" + field.propName.charAt(0).toUpperCase() + field.propName.slice(1);
                    }

                    // determine field type
                    if (field.type === "number") {
                        props.min = field.min;
                        props.max = field.max;

                        componentToCreate = components.NumberField;

                    } else if (field.type === "slider") {
                        props.minVal = field.min;
                        props.maxVal = field.max;

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
                    paramsSubRack.fields.push(fieldObj)
                }

                // relink props
                delay(30, paramsRack.finishedGenerating);
            }
        }
    }
}
