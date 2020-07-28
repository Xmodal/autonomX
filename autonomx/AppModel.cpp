// Copyright 2020, Xmodal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "AppModel.h"

AppModel::AppModel() {
    // init data (list form)
    generatorsList = QSharedPointer<QList<QSharedPointer<Generator>>>(new QList<QSharedPointer<Generator>>());
    generatorFacadesList = QSharedPointer<QList<QSharedPointer<GeneratorFacade>>>(new QList<QSharedPointer<GeneratorFacade>>());

    // init data (hashmap form)
    generatorsHash = QSharedPointer<QHash<int, QSharedPointer<Generator>>>(new QHash<int, QSharedPointer<Generator>>());
    generatorFacadesHash = QSharedPointer<QHash<int, QSharedPointer<GeneratorFacade>>>(new QHash<int, QSharedPointer<GeneratorFacade>>());

    // init data (unique elements)
    QSharedPointer<GeneratorModel> generatorModel = QSharedPointer<GeneratorModel>(new GeneratorModel(generatorFacadesList));
}

void AppModel::startThreads() {

}

QThread* AppModel::getComputeThread() const {
    return computeThread.data();
}

QThread* AppModel::getOscThread() const {
    return oscThread.data();
}

void AppModel::createGenerator() {
    generatorModel->relinkAliasConnections();
}

void AppModel::deleteGenerator(int id) {
    generatorModel->relinkAliasConnections();
}

bool AppModel::validateNewGeneratorName(QString name) {
    bool valid = true;
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList.get()->begin(); it != generatorsList.get()->end() && valid; it++) {
        valid = name != it->data()->getName();
    }
    return valid;
}

Generator* AppModel::getGenerator(int id) const {
    if(!generatorsHash->contains(id)) {
        throw std::runtime_error("generator does not exist");
    }
    return generatorsHash->value(id).data();
}

GeneratorFacade* AppModel::getGeneratorFacade(int id) const {
    if(!generatorFacadesHash->contains(id)) {
        throw std::runtime_error("generator facade does not exist");
    }
    return generatorFacadesHash->value(id).data();
}

GeneratorModel* AppModel::getGeneratorModel() const {
    return generatorModel.data();
}
