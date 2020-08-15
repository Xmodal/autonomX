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

#include "GeneratorRegionSet.h"

GeneratorRegionSet::GeneratorRegionSet() {

}

GeneratorRegion* GeneratorRegionSet::getRegion(int index) {
    return regionList.at(index).data();
}

int GeneratorRegionSet::getRegionCount() {
    return regionList.size();
}

void GeneratorRegionSet::addRegion(GeneratorRegion region) {
    // something about this feels wrong
    regionList.append(QSharedPointer<GeneratorRegion>(&region));
}

void GeneratorRegionSet::deleteRegion(int index) {
    regionList.removeAt(index);
}

void GeneratorRegionSet::writeRegion(QVariant value, GeneratorRegionModel::GeneratorRegionRoles role, int index) {
    // check if the key exists in the hash map
    if(GeneratorRegionModel::roleMap.contains(role)) {
        QSharedPointer<GeneratorRegion> region = regionList.at(index);
        region->setProperty(GeneratorRegionModel::roleMap.value(role), value);
    }
}
