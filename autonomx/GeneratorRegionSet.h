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

#include "GeneratorRegion.h"
#include "GeneratorRegionModel.h"

class GeneratorRegionSet : public QObject{
    Q_OBJECT
public:
    GeneratorRegionSet();

    GeneratorRegion* getRegion(int index);
    int getRegionCount();
private:
    QList<QSharedPointer<GeneratorRegion>> regionList;
    bool flagDebug = false;
public slots:
    void addRegion(int x, int y, int width, int height);
    void deleteRegion(int index);

    void writeRegion(QVariant value, int role, int index);
};

