import QtQuick 2.0

/**
 * Model for the generators.
 */
ListModel {
    id: generatorsModel

    function _find(model, criteria) {
        for (var i = 0; i < model.count; ++i) {
            if (criteria(model.get(i))) {
                return model.get(i);
            }
        }
        return null;
    }
    function findGenerator(generator_identifier) {
        return _find(generatorsModel, function(item) {
            return item.identifier === generator_identifier;
        });
    }

    ListElement {
        identifier: "game_of_life"
        description_fr: "Description en français"
        description_en: "Description in English"
    }
}

