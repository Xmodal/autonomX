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

#include "GeneratorRegion.h"

GeneratorRegion::GeneratorRegion(QRect rect, double intensity) {
    this->rect = rect;
    this->intensity = intensity;
}

GeneratorRegion::GeneratorRegion(const GeneratorRegion& region) {
    this->rect = region.getRect();
    this->intensity = region.getIntensity();
}

QRect GeneratorRegion::getRect() const {
    return rect;
}

double GeneratorRegion::getIntensity() const {
    return intensity;
}

void GeneratorRegion::writeRect(QRect rect) {
    if(this->rect == rect) {
        return;
    }

    this->rect = rect;
    emit rectChanged(rect);
}

void GeneratorRegion::writeIntensity(double intensity) {
    if(this->intensity == intensity) {
        return;
    }
}
