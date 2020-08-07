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

#include "GeneratorLattice.h"
#include <QDebug>

QQuickFramebufferObject::Renderer * GeneratorLattice::createRenderer() const {
    connect(this, &GeneratorLattice::visibleChanged, this, &QQuickFramebufferObject::update);
    QQuickFramebufferObject::Renderer * renderer = new GeneratorLatticeRenderer();
    return renderer;
}

int GeneratorLattice::getGeneratorID() {
    return generatorID;
}

float GeneratorLattice::getSquareInPixels() {
    return squareInPixels;
}

QVector4D GeneratorLattice::getMask() {
    return mask;
}

float GeneratorLattice::getMaskAlpha() {
    return maskAlpha;
}

void GeneratorLattice::writeSquareInPixels(float squareInPixels) {
    if(this->squareInPixels == squareInPixels) {
        return;
    }

    this->squareInPixels = squareInPixels;
    emit squareInPixelsChanged(squareInPixels);

    // request a syncrhonize call to GeneratorLatticeRenderer
    update();
}

void GeneratorLattice::writeMask(QVector4D mask) {
    if(this->mask == mask) {
        return;
    }

    if(flagDebug) {
        qDebug() << "writeMask (GeneratorLattice):\t" << mask;
    }

    this->mask = mask;
    emit maskChanged(mask);

    // request a syncrhonize call to GeneratorLatticeRenderer
    update();
}

void GeneratorLattice::writeMaskAlpha(float maskAlpha) {
    if(this->maskAlpha == maskAlpha) {
        return;
    }

    if(flagDebug) {
        qDebug() << "writeMaskAlpha (GeneratorLattice):\t" << maskAlpha;
    }

    this->maskAlpha = maskAlpha;
    emit maskAlphaChanged(maskAlpha);

    // request a syncrhonize call to GeneratorLatticeRenderer
    update();
}

void GeneratorLattice::writeGeneratorID(int generatorID) {
    if(this->generatorID == generatorID) {
        return;
    }

    if(flagDebug) {
        qDebug() << "writeGeneratorID (GeneratorLattice):\t" << generatorID;
    }

    this->generatorID = generatorID;
    emit generatorIDChanged(generatorID);

    // request a syncrhonize call to GeneratorLatticeRenderer
    update();
}

