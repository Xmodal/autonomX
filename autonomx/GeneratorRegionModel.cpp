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

#include "GeneratorRegionModel.h"

GeneratorRegionModel::GeneratorRegionModel() {
    // set ownership of the GeneratorFacade to C++ so that QML doesn't try to take over garbage collection duties, resulting in a double free
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (GeneratorRegionModel)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();

    }

    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(0, 0, 10, 10), 0.0));

    // relink connections
    relinkConnections();
}

GeneratorRegionModel::~GeneratorRegionModel() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (GeneratorRegionModel)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();

    }
}

int GeneratorRegionModel::rowCount(const QModelIndex& parent) const {
    return regionList.size();
}

int GeneratorRegionModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant GeneratorRegionModel::data(const QModelIndex &index, int role) const {
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

bool GeneratorRegionModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(flagDebug) {
        qDebug() << "setData (GeneratorRegionModel) value: " << value;
    }

    if(index.isValid()) {
        // check if the index is valid
        if(index.column() == 0 && index.row() >= 0 && index.row() < regionList.size()) {
            // check if the key exists in the hash map
            if(GeneratorRegion::roleMap.contains(role)) {
                regionList.at(index.row())->setProperty(GeneratorRegion::roleMap.value(role), value);
                emit writeRegionFromModel(value, role, index.row());
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

QHash<int, QByteArray> GeneratorRegionModel::roleNames() const {
    return GeneratorRegion::roleMap;
}

GeneratorRegion* GeneratorRegionModel::at(int index) {
    if (index < 0) return nullptr;
    return regionList.at(index).data();
}

void GeneratorRegionModel::addRegion(int x, int y, int width, int height) {
    if(flagDebug) {
        qDebug() << "addRegion (GeneratorRegionModel)";
    }

    // tell the model we are about to add a row at the end of the list
    int index = regionList.size();
    beginInsertRows(QModelIndex(), index, index);

    // create the region and add it to the list
    QSharedPointer<GeneratorRegion> region = QSharedPointer<GeneratorRegion>(new GeneratorRegion(QRect(x, y, width, height), 0.0));
    regionList.append(region);

    // tell the model we are done adding rows
    endInsertRows();

    // relink connections
    relinkConnections();

    // emit signal so that backend updates
    emit addRegionFromModel(x, y, width, height);
}

void GeneratorRegionModel::deleteRegion(int index) {
    if(flagDebug) {
        qDebug() << "deleteRegion (GeneratorRegionModel)";
    }

    // tell the model we are about to remove a row
    beginRemoveRows(QModelIndex(), index, index);

    // remove from list
    regionList.removeAt(index);

    // tell the model we are done removing rows
    endRemoveRows();

    // relink connections
    relinkConnections();

    // emit signal so that backend updates
    emit deleteRegionFromModel(index);
}

void GeneratorRegionModel::writeRegion(QVariant value, int role, int index) {
    if(flagDebug) {
        qDebug() << "writeRegion (GeneratorRegionModel) value: " << value << "\t role: " << role << "\t index: " << index;
    }

    // check if the index is valid
    if(index >= 0 && index < regionList.size()) {
        // check if the key exists in the hash map
        if(GeneratorRegion::roleMap.contains(role)) {
            QVariant previousValue = regionList.at(index)->property(GeneratorRegion::roleMap.value(role));
            // check if value changed
            if(previousValue == value) {
                if(flagDebug) {
                    qDebug() << "writeRegion (GeneratorRegionModel) canceled because of redundant change";
                }
                return;
            }
            regionList.at(index)->setProperty(GeneratorRegion::roleMap.value(role), value);
            emit dataChanged(QAbstractListModel::index(index), QAbstractListModel::index(index), {role});
            if(flagDebug) {
                qDebug() << "writeRegion (GeneratorRegionModel): success";
            }
            return;
        } else if(flagDebug) {
            qDebug() << "writeRegion (GeneratorRegionModel) role does not exist";
        }
    } else if(flagDebug) {
        qDebug() << "writeRegion (GeneratorRegionModel) index out of range";
    }
}

void GeneratorRegionModel::createConnections() {
    if(flagDebug) {
        qDebug() << "createConnections (GeneratorRegionModel)";
    }

    for(int i = 0; i < regionList.count(); i++) {
        GeneratorRegion* generatorRegion = regionList.at(i).data();

        // create the connection
        QMetaObject::Connection connection = connect(generatorRegion, &GeneratorRegion::valueChanged, this, [=](const QString &key, const QVariant &value) {
            if(flagDebug) {
                qDebug() << "lambda (GeneratorRegionModel) key: " << key << "\t value: " << value << "\t index: " << i;
            }

            // this ridicoulous shit is necessary to get QString to properly convert to QByteArray. for some reason QString.toUtf8().toBase64() produces garbage results
            QByteArray keyBuffer;
            keyBuffer.append(key);

            emit writeRegionFromModel(value, GeneratorRegion::roleMap.key(keyBuffer), i);
        });

        connections.append(connection);
    }
}

void GeneratorRegionModel::deleteConnections() {
    if(flagDebug) {
        qDebug() << "deleteConnections (GeneratorRegionModel)";
    }

    // delete the alias contexts, destroying connections as a result
    for(QList<QMetaObject::Connection>::iterator it = connections.begin(); it != connections.end(); it++) {
        disconnect(*it);
    }
    connections.clear();
}

void GeneratorRegionModel::relinkConnections() {
    if(flagDebug) {
        qDebug() << "relinkConnections (GeneratorModel)";
    }
    deleteConnections();
    createConnections();
}
