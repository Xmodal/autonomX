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

#include <QQmlEngine>

#include "GeneratorRegion.h"

GeneratorRegion::GeneratorRegion(QRect rect, double intensity, int type) {
    // set ownership of the GeneratorRegion to C++ so that QML doesn't try to take over garbage collection duties, resulting in a double free
    // this is necessary because GeneratorModel will return pointers to GeneratorRegion
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    this->rect = rect;
    this->intensity = intensity;
    this->type = type;
}

GeneratorRegion::GeneratorRegion(const GeneratorRegion& region) {
    // set ownership of the GeneratorRegion to C++ so that QML doesn't try to take over garbage collection duties, resulting in a double free
    // this is necessary because GeneratorModel will return pointers to GeneratorRegion
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    this->rect = region.getRect();
    this->intensity = region.getIntensity();
    this->type = region.getType();
}

QRect GeneratorRegion::getRect() const {
    return rect;
}

double GeneratorRegion::getIntensity() const {
    return intensity;
}

int GeneratorRegion::getType() const {
    return type;
}

void GeneratorRegion::writeSilentRect(QRect rect) {
    if(this->rect == rect) {
        return;
    }

    this->rect = rect;
    emit rectChanged(rect);
}

void GeneratorRegion::writeSilentIntensity(double intensity) {
    if(this->intensity == intensity) {
        return;
    }

    this->intensity = intensity;
    emit intensityChanged(intensity);
}

void GeneratorRegion::writeSilentType(int type) {
    if(this->type == type) {
        return;
    }

    this->type = type;
    emit typeChanged(type);
}

void GeneratorRegion::writeMirroredRect(QRect rect) {
    if(this->rect == rect) {
        return;
    }

    this->rect = rect;
    emit rectChanged(rect);
    emit valueChanged("rect", QVariant(rect));
}

void GeneratorRegion::writeMirroredIntensity(double intensity) {
    if(this->intensity == intensity) {
        return;
    }

    this->intensity = intensity;
    emit intensityChanged(intensity);
    emit valueChanged("intensity", QVariant(intensity));
}

void GeneratorRegion::writeMirroredType(int type) {
    if(this->type == type) {
        return;
    }

    this->type = type;
    emit typeChanged(type);
    emit valueChanged("type", QVariant(type));
}
