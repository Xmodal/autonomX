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
}

GeneratorRegion* GeneratorRegionSet::getRegion(int index) {
    return regionList.at(index).data();
}

int GeneratorRegionSet::getRegionCount() {
    return regionList.size();
}

void GeneratorRegionSet::addRegion(GeneratorRegion region) {
    if(flagDebug) {
        qDebug() << "addRegion (GeneratorRegionSet)";
    }

    // something about this feels wrong
    regionList.append(QSharedPointer<GeneratorRegion>(&region));
}

void GeneratorRegionSet::deleteRegion(int index) {
    if(flagDebug) {
        qDebug() << "deleteRegion (GeneratorRegionSet)";
    }

    regionList.removeAt(index);
}

void GeneratorRegionSet::writeRegion(QVariant value, int role, int index) {
    if(flagDebug) {
        qDebug() << "writeRegion (GeneratorRegionSet) value: " << value;
    }

    // check if the key exists in the hash map
    if(GeneratorRegionModel::roleMap.contains((GeneratorRegionModel::GeneratorRegionRoles) role)) {
        QSharedPointer<GeneratorRegion> region = regionList.at(index);
        region->setProperty(GeneratorRegionModel::roleMap.value((GeneratorRegionModel::GeneratorRegionRoles) role), value);
        if(flagDebug) {
            qDebug() << "writeRegion (GeneratorRegionSet) successful";
        }
    } else {
        if(flagDebug) {
            qDebug() << "writeRegion (GeneratorRegionSet) failed";
        }
    }
}
