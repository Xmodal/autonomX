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

#include "GeneratorRegionSet.h"

GeneratorRegionSet::GeneratorRegionSet() {
    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(0, 0, 10, 10), 0.0));

    // relink connections
    relinkConnections();
}

GeneratorRegionSet::~GeneratorRegionSet() {

}

GeneratorRegion* GeneratorRegionSet::getRegion(int index) {
    return regionList.at(index).data();
}

int GeneratorRegionSet::getRegionCount() {
    return regionList.size();
}

void GeneratorRegionSet::addRegion(int x, int y, int width, int height) {
    if(flagDebug) {
        qDebug() << "addRegion (GeneratorRegionSet)";
    }

    // create copy of the region as a shared pointer and add to list
    QSharedPointer<GeneratorRegion> region = QSharedPointer<GeneratorRegion>(new GeneratorRegion(QRect(x, y, width, height), 0.0));
    regionList.append(QSharedPointer<GeneratorRegion>(region));

    // relink connections
    relinkConnections();
}

void GeneratorRegionSet::deleteRegion(int index) {
    if(flagDebug) {
        qDebug() << "deleteRegion (GeneratorRegionSet)";
    }

    regionList.removeAt(index);

    // relink connections
    relinkConnections();
}

void GeneratorRegionSet::writeRegion(QVariant value, int role, int index) {
    if(flagDebug) {
        qDebug() << "writeRegion (GeneratorRegionSet) value: " << value;
    }

    // check if index is valid
    if(index >= 0 && index < regionList.size()) {
        // check if the key exists in the hash map
        if(GeneratorRegionModel::roleMap.contains((GeneratorRegionModel::GeneratorRegionRoles) role)) {
            QSharedPointer<GeneratorRegion> region = regionList.at(index);
            QVariant previousValue = region->property(GeneratorRegionModel::roleMap.value(role));
            // check if value changed
            if(previousValue == value) {
                if(flagDebug) {
                    qDebug() << "writeRegion (GeneratorRegionSet) canceled because of redundant change";
                }
                return;
            }
            region->setProperty(GeneratorRegionModel::roleMap.value(role), value);
            if(flagDebug) {
                qDebug() << "writeRegion (GeneratorRegionSet) successful";
            }
            return;
        }
    }

    if(flagDebug) {
        qDebug() << "writeRegion (GeneratorRegionSet) failed";
    }
}

void GeneratorRegionSet::createConnections() {
    if(flagDebug) {
        qDebug() << "createConnections (GeneratorModel)";
    }

    for(int i = 0; i < regionList.count(); i++) {
        GeneratorRegion* generatorRegion = regionList.at(i).data();

        // create the connection
        QMetaObject::Connection connection = connect(generatorRegion, &GeneratorRegion::valueChanged, this, [=](const QString &key, const QVariant &value) {
            emit writeRegion(value, GeneratorRegionModel::roleMap.key(key.toUtf8().toBase64()), i);
        });

        connections.append(connection);
    }
}

void GeneratorRegionSet::deleteConnections() {
    if(flagDebug) {
        qDebug() << "deleteConnections (GeneratorRegionModel)";
    }

    // delete the alias contexts, destroying connections as a result
    for(QList<QMetaObject::Connection>::iterator it = connections.begin(); it != connections.end(); it++) {
        disconnect(*it);
    }
    connections.clear();
}

void GeneratorRegionSet::relinkConnections() {
    if(flagDebug) {
        qDebug() << "relinkConnections (GeneratorModel)";
    }
    deleteConnections();
    createConnections();
}
