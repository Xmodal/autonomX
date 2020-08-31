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
#include <QList>
#include <QSharedPointer>
#include <QMetaObject>

#include "GeneratorRegion.h"
#include "GeneratorRegionModel.h"

class GeneratorRegionSet : public QObject{
    Q_OBJECT
public:
    GeneratorRegionSet(int type); // argument is region type. 0 = input, 1 = output
    ~GeneratorRegionSet();

    // iterates over all members of the GeneratorRegion list and creates connections for them.
    void createConnections();
    // deletes all connections from GeneratorRegion.
    void deleteConnections();
    // this simply deletes then creates the connections.
    void relinkConnections();

    GeneratorRegion* getRegion(int index);
    int getRegionCount();
private:
    void initializeAsInput();
    void initializeAsOutput();

    QList<QMetaObject::Connection> connections;
    QList<QSharedPointer<GeneratorRegion>> regionList;
    bool flagDebug = false;
    const int type; // region type. 0 = input, 1 = output
public slots:
    // these should only ever be called from a RegionModel. changes will not propagate back to the RegionModel if called directly
    void addRegion(int x, int y, int width, int height);
    void deleteRegion(int index);
    void writeRegion(QVariant value, int role, int index);
signals:
    // this can be called directly and will propagate back to its RegionModel
    void writeRegionFromSet(QVariant value, int role, int index);
};

