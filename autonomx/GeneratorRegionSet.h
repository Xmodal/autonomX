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

#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QModelIndex>
#include <QList>
#include <QSharedPointer>
#include <QMetaObject>

#include "GeneratorRegion.h"

class GeneratorRegionSet : public QAbstractListModel {
    Q_OBJECT
public:
    GeneratorRegionSet(int type); // argument is region type. 0 = input, 1 = output
    ~GeneratorRegionSet();

    // called in very specific edge cases to fill up the lattice
    // with predetermined I/O region structures;
    // this should     be called from the GUI
    // this should NOT be called from the serialization process
    void initialize();

    // iterates over all members of the GeneratorRegion list and creates connections for them.
    void createConnections();
    // deletes all connections from GeneratorRegion.
    void deleteConnections();
    // this simply deletes then creates the connections.
    void relinkConnections();

    // delete all regions
    void deleteAllRegions();

    // serialization
    void readJson(const QJsonArray &json);
    void writeJson(QJsonArray &json) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void populate();
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE GeneratorRegion* at(int index);

private:
    void initializeAsInput();
    void initializeAsOutput();

    QList<QMetaObject::Connection> connections;
    QList<QSharedPointer<GeneratorRegion>> regionList;
    bool flagDebug = false;
    const int type; // region type. 0 = input, 1 = output

public slots:
    Q_INVOKABLE void addRegion(int x, int y, int width, int height);
    Q_INVOKABLE void addRegion(QRect rect);
    Q_INVOKABLE void deleteRegion(int index);
    Q_INVOKABLE void writeRegion(QVariant value, int role, int index);

signals:
    // [DEPRECATED] this can be called directly and will propagate back to its RegionModel
    // void writeRegionFromSet(QVariant value, int role, int index);
    void rowCountChanged(int rowCount);
};

