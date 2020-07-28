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

#include "GeneratorModel.h"
#include <chrono>
#include <QThread>
#include <QVector>
#include <QDebug>

GeneratorModel::GeneratorModel(QSharedPointer<QList<QSharedPointer<GeneratorFacade>>> generatorGeneratorFacades) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (GeneratorModel)\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->generatorGeneratorFacades = generatorGeneratorFacades;

    createAliasConnections();
}

GeneratorModel::~GeneratorModel() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (GeneratorModel)\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
}

void GeneratorModel::updateValueFromAlias(const QString &key, const QVariant &value, int modelIndex) {
    QVector<int> roles;
    bool unrecognized = false;

    int role = roleMap.key(key.toUtf8(), -1);
    // check to see if the value exists in the hash map
    if (role == -1) unrecognized = true;
    else roles = { role };

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        QByteArray keyArray = key.toLocal8Bit();
        char* keyBuffer = keyArray.data();

        QByteArray valueArray = value.toString().toLocal8Bit();
        char* valueBuffer = valueArray.data();

        qDebug() << "lambda (" << keyBuffer << "):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\t value = " << valueBuffer << (unrecognized ? " (unrecognized)" : "");
    }

    if(!unrecognized) {
        emit dataChanged(index(modelIndex), index(modelIndex), roles);
    }
}

void GeneratorModel::createAliasConnections() {
    for(int i = 0; i < generatorGeneratorFacades.get()->count(); i++) {
        // update model every time something is changed
        connect(generatorGeneratorFacades.get()->at(i).get(), &GeneratorFacade::valueChangedFromAlias, this, [=](const QString &key, const QVariant &value) {
            emit updateValueFromAlias(key, value, i);
        });
    }
}

void GeneratorModel::deleteAliasConnections() {
    disconnect(this);
}

void GeneratorModel::relinkAliasConnections() {
    deleteAliasConnections();
    createAliasConnections();
}

int GeneratorModel::rowCount(const QModelIndex& parent) const {
    return generatorGeneratorFacades.get()->size();
}

int GeneratorModel::columnCount(const QModelIndex& parent) const {
  return 1;
}

QVariant GeneratorModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid())
        return QVariant();

    // check if the index is valid
    if(index.column() == 0 && index.row() >= 0 && index.row() < generatorGeneratorFacades.get()->size()) {
        // check if the key exists in the hash map
        if(roleMap.contains(role))
            return generatorGeneratorFacades.get()->at(index.row())->value(roleMap.value(role));
    }

    return QVariant();
}

QHash<int, QByteArray> GeneratorModel::roleNames() const {
    return roleMap;
}

GeneratorFacade * GeneratorModel::at(int index) {
    if (index < 0) return nullptr;
    return generatorGeneratorFacades.get()->at(index).get();
}
