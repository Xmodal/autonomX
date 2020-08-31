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

GeneratorRegionSet::GeneratorRegionSet(int type) : type(type) {
    if(flagDebug) {
        qDebug() << "constructor (GeneratorRegionSet)";
    }

    if(type == 0) {
        initializeAsInput();
    } else {
        initializeAsOutput();
    }
}

GeneratorRegionSet::~GeneratorRegionSet() {
    if(flagDebug) {
        qDebug() << "destructor (GeneratorRegionSet)";
    }
}

void GeneratorRegionSet::initializeAsInput() {
    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(1, 3, 3, 3), 0.0, 0));
    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(6, 3, 3, 3), 0.0, 0));
    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(11, 3, 3, 3), 0.0, 0));
    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(16, 3, 3, 3), 0.0, 0));
    createConnections();
}

void GeneratorRegionSet::initializeAsOutput() {
    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(1, 14, 3, 3), 0.0, 1));
    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(6, 14, 3, 3), 0.0, 1));
    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(11, 14, 3, 3), 0.0, 1));
    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(16, 14, 3, 3), 0.0, 1));
    createConnections();
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
    QSharedPointer<GeneratorRegion> region = QSharedPointer<GeneratorRegion>(new GeneratorRegion(QRect(x, y, width, height), 0.0, type));
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

            // this ridicoulous shit is necessary to get QString to properly convert to QByteArray. for some reason QString.toUtf8().toBase64() produces garbage results
            QByteArray keyBuffer;
            keyBuffer.append(key);

            emit writeRegionFromSet(value, GeneratorRegion::roleMap.key(keyBuffer), i);
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
