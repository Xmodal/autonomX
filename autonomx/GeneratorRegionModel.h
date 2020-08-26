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
#include <QSharedPointer>
#include <QList>
#include <QMetaObject>

#include "GeneratorRegion.h"

class GeneratorRegionModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum GeneratorRegionRoles {
        RectRole = Qt::UserRole + 1,
        IntensityRole
    };

    static inline const QHash<int, QByteArray> roleMap = {
        {RectRole, "rect"},
        {IntensityRole, "intensity"},
    };

    GeneratorRegionModel();
    ~GeneratorRegionModel();

    // iterates over all members of the GeneratorRegion list and creates connections for them.
    void createConnections();
    // deletes all connections from GeneratorRegion.
    void deleteConnections();
    // this simply deletes then creates the connections.
    void relinkConnections();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void populate();
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE GeneratorRegion* at(int index);
private:
    QList<QMetaObject::Connection> connections;
    QList<QSharedPointer<GeneratorRegion>> regionList;
    bool flagDebug = false;
public slots:
    // these can be called from QML. writeRegion is also used by GeneratorRegionSet to update Region's intensity property
    Q_INVOKABLE void addRegion(int x, int y, int width, int height);
    Q_INVOKABLE void deleteRegion(int index);
    Q_INVOKABLE void writeRegion(QVariant value, int role, int index);
signals:
    void addRegionFromModel(int x, int y, int width, int height);
    void deleteRegionFromModel(int index);
    void writeRegionFromModel(QVariant value, int role, int index);
};
