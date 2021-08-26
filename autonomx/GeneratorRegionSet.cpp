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

#include <QDebug>
#include <QThread>
#include <QQmlEngine>
#include <QJsonObject>
#include <QJsonArray>

#include "GeneratorRegionSet.h"

GeneratorRegionSet::GeneratorRegionSet(int type) : type(type) {
    if(flagDebug) {
        qDebug() << "constructor (GeneratorRegionSet)";
    }

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

GeneratorRegionSet::~GeneratorRegionSet() {
    if(flagDebug) {
        qDebug() << "destructor (GeneratorRegionSet)";
    }
}

void GeneratorRegionSet::initialize()
{
    qDebug() << "GeneratorRegionSet::initialize 1";
    // first, empty the list
    deleteAllRegions();

    qDebug() << "GeneratorRegionSet::initialize 2";

    // initialize as input or output
    if(type == 0) {
        qDebug() << "GeneratorRegionSet::initialize 2.1";
        initializeAsInput();
    } else {
        qDebug() << "GeneratorRegionSet::initialize 2.2";
        initializeAsOutput();
    }

    qDebug() << "GeneratorRegionSet::initialize 3";
}

void GeneratorRegionSet::initializeAsInput() {

    qDebug() << "GeneratorRegionSet::initializeAsInput 1";
    // optimized for 20x20 lattice size
    this->addRegion(3, 7, 8, 8);
    qDebug() << "GeneratorRegionSet::initializeAsInput 1.1";
    this->addRegion(15, 7, 8, 8);
    qDebug() << "GeneratorRegionSet::initializeAsInput 1.2";
    this->addRegion(27, 7, 8, 8);
    qDebug() << "GeneratorRegionSet::initializeAsInput 1.3";
    this->addRegion(39, 7, 8, 8);

    qDebug() << "GeneratorRegionSet::initializeAsInput 2";
}

void GeneratorRegionSet::initializeAsOutput() {
    // optimized for 20x20 lattice size
    this->addRegion(3, 35, 8, 8);
    this->addRegion(15, 35, 8, 8);
    this->addRegion(27, 35, 8, 8);
    this->addRegion(39, 35, 8, 8);
}

int GeneratorRegionSet::rowCount(const QModelIndex& parent) const {
    return regionList.size();
}

int GeneratorRegionSet::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant GeneratorRegionSet::data(const QModelIndex &index, int role) const {
    if(flagDebug) {
        qDebug() << "data (GeneratorRegionModel)";
    }

    if(index.isValid()) {
        // check if the index is valid
        if(index.column() == 0 && index.row() >= 0 && index.row() < regionList.size()) {
            // check if the key exists in the hash map
            if(GeneratorRegion::roleMap.contains(role))
                return regionList.at(index.row())->property(GeneratorRegion::roleMap.value(role));

        }
    }

    if(flagDebug) {
        qDebug() << "data (GeneratorRegionModel) failed to find valid entry";
    }

    return QVariant();
}

bool GeneratorRegionSet::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(flagDebug) {
        qDebug() << "setData (GeneratorRegionModel) value: " << value;
    }

    if(index.isValid()) {
        // check if the index is valid
        if(index.column() == 0 && index.row() >= 0 && index.row() < regionList.size()) {
            // check if the key exists in the hash map
            if(GeneratorRegion::roleMap.contains(role)) {
                regionList.at(index.row())->setProperty(GeneratorRegion::roleMap.value(role), value);
                // emit writeRegionFromModel(value, role, index.row());
                if(flagDebug) {
                    qDebug() << "setData (GeneratorRegionModel): signal emitted";
                }
                return true;
            }
        }
    }

    if(flagDebug) {
        qDebug() << "setData (GeneratorRegionModel) failed to find valid entry";
    }

    return false;
}

QHash<int, QByteArray> GeneratorRegionSet::roleNames() const {
    return GeneratorRegion::roleMap;
}

GeneratorRegion* GeneratorRegionSet::at(int index) {
    if (index < 0 || index >= regionList.size()) return nullptr;

    return regionList.at(index).data();
}

void GeneratorRegionSet::addRegion(int x, int y, int width, int height) {
    if(flagDebug) {
        qDebug() << "addRegion (GeneratorRegionSet)";
    }

    qDebug() << "GeneratorRegionSet::addRegion 1";

    // tell the model we are about to add a row at the end of the list
    int index = regionList.size();
    qDebug() << "GeneratorRegionSet::addRegion 1.1";
    beginInsertRows(QModelIndex(), index, index);
    qDebug() << "GeneratorRegionSet::addRegion 1.2";
    // create copy of the region as a shared pointer and add to list
    QSharedPointer<GeneratorRegion> region = QSharedPointer<GeneratorRegion>(new GeneratorRegion(QRect(x, y, width, height), 0.0, type));
    qDebug() << "GeneratorRegionSet::addRegion 1.3";
    regionList.append(region);
    qDebug() << "GeneratorRegionSet::addRegion 1.4";
    // tell the model we are done adding rows
    endInsertRows();
    qDebug() << "GeneratorRegionSet::addRegion 1.5";

    // relink connections
    relinkConnections();

    qDebug() << "GeneratorRegionSet::addRegion 2";
    // emit signal so that backend updates
    emit rowCountChanged(regionList.size());
    // emit addRegionFromSet(x, y, width, height);
}

void GeneratorRegionSet::addRegion(QRect rect)
{
    addRegion(rect.x(), rect.y(), rect.width(), rect.height());
}

void GeneratorRegionSet::deleteRegion(int index) {
    if(flagDebug) {
        qDebug() << "deleteRegion (GeneratorRegionSet)";
    }

    // tell the model we are about to remove a row
    beginRemoveRows(QModelIndex(), index, index);
    // remove from list
    regionList.removeAt(index);
    // tell the model we are done removing rows
    endRemoveRows();

    emit rowCountChanged(regionList.size());

    // relink connections
    relinkConnections();

    // emit signal so that backend updates
    // emit deleteRegionFromSet(index);
}

void GeneratorRegionSet::writeRegion(QVariant value, int role, int index) {
    if(flagDebug) {
        qDebug() << "writeRegion (GeneratorRegionSet) value: " << value << "\t role: " << role << "\t index: " << index;
    }

    // check if index is valid
    if(index >= 0 && index < regionList.size()) {
        // check if the key exists in the hash map
        if(GeneratorRegion::roleMap.contains((GeneratorRegion::GeneratorRegionRoles) role)) {
            QSharedPointer<GeneratorRegion> region = regionList.at(index);
            QVariant previousValue = region->property(GeneratorRegion::roleMap.value(role));
            // check if value changed
            if(previousValue == value) {
                if(flagDebug) {
                    qDebug() << "writeRegion (GeneratorRegionSet) canceled because of redundant change";
                }
                return;
            }
            region->setProperty(GeneratorRegion::roleMap.value(role), value);
            if(flagDebug) {
                qDebug() << "writeRegion (GeneratorRegionSet) success";
            }
            return;
        } else if(flagDebug) {
            qDebug() << "writeRegion (GeneratorRegionSet) role does not exist";
        }
    } else if(flagDebug) {
        qDebug() << "writeRegion (GeneratorRegionSet) index out of range";
    }
}

void GeneratorRegionSet::createConnections() {
    if(flagDebug) {
        qDebug() << "createConnections (GeneratorRegionSet)";
    }

    for(int i = 0; i < regionList.count(); i++) {
        GeneratorRegion* generatorRegion = regionList.at(i).data();

        // create the connection
        QMetaObject::Connection connection = connect(generatorRegion, &GeneratorRegion::valueChanged, this, [=](const QString &key, const QVariant &value) {
            if(flagDebug) {
                qDebug() << "lambda (GeneratorRegionSet) key: " << key << "\t value: " << value << "\t index: " << i;
            }

            // this ridicoulous crap is necessary to get QString to properly convert to QByteArray. for some reason QString.toUtf8().toBase64() produces garbage results
            QByteArray keyBuffer;
            keyBuffer.append(key);

            // trigger QML update
            emit dataChanged(index(i), index(i), QVector<int>{GeneratorRegion::roleMap.key(keyBuffer)});
        });

        connections.append(connection);
    }
}

void GeneratorRegionSet::deleteConnections() {
    if(flagDebug) {
        qDebug() << "deleteConnections (GeneratorRegionSet)";
    }

    // delete the alias contexts, destroying connections as a result
    for(QList<QMetaObject::Connection>::iterator it = connections.begin(); it != connections.end(); it++) {
        disconnect(*it);
    }
    connections.clear();
}

void GeneratorRegionSet::relinkConnections() {
    if(flagDebug) {
        qDebug() << "relinkConnections (GeneratorRegionSet)";
    }
    deleteConnections();
    createConnections();
}

void GeneratorRegionSet::readJson(const QJsonArray &json)
{
    // we're assuming that regionList is empty (meaning size = 0) here
    // because readJson is only called when loading a project,
    // which means that both I/O sets are already empty.

    // first off, we announce a model reset
    beginResetModel();

    // then we populate the regionList array
    for (int i = 0; i < json.size(); i++) {
        // create new region
        QSharedPointer<GeneratorRegion> region = QSharedPointer<GeneratorRegion>(new GeneratorRegion(QRect(0, 0, 0, 0), 0.0, type));
        region->readJson(json[i].toObject());

        regionList.append(region);
    }

    // announce end of modifications
    endResetModel();
    // signal row count change
    rowCountChanged(regionList.count());
    // create connections with regions
    createConnections();
}

void GeneratorRegionSet::writeJson(QJsonArray &json) const
{
    for (int i = 0; i < regionList.count(); i++) {
        GeneratorRegion* region = regionList.at(i).data();

        // write to obj
        QJsonObject obj;
        region->writeJson(obj);

        // append
        json.append(obj);
    }
}

void GeneratorRegionSet::deleteAllRegions() {
    // anticipation
    beginResetModel();

    // execution
    deleteConnections();
    regionList.clear();

    // release
    endResetModel();
}
