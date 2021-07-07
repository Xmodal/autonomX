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

#include <QQuickFramebufferObject>

#include "GeneratorLatticeRenderer.h"

class GeneratorLattice : public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(int generatorID READ getGeneratorID WRITE writeGeneratorID NOTIFY generatorIDChanged)
    Q_PROPERTY(float squareInPixels READ getSquareInPixels WRITE writeSquareInPixels NOTIFY squareInPixelsChanged)
    Q_PROPERTY(QVector4D mask READ getMask WRITE writeMask NOTIFY maskChanged)
    Q_PROPERTY(float maskAlpha READ getMaskAlpha WRITE writeMaskAlpha NOTIFY maskAlphaChanged)
    Q_PROPERTY(QVector2D pan READ getPan WRITE writePan NOTIFY panChanged)
public:
    GeneratorLattice();
    ~GeneratorLattice();

    QQuickFramebufferObject::Renderer * createRenderer() const;
    int getGeneratorID();
    float getSquareInPixels();
    int getContainerWidthInPixels();
    int getContainerHeightInPixels();
    QVector4D getMask();
    float getMaskAlpha();
    QVector2D getPan();

    void writeGeneratorID(int generatorID);
    void writeSquareInPixels(float squareInPixels);
    void writeMask(QVector4D mask);
    void writeMaskAlpha(float maskAlpha);
    void writePan(QVector2D pan);
signals:
    void generatorIDChanged(int generatorID);
    void squareInPixelsChanged(float squareInPixels);
    void maskChanged(QVector4D mask);
    void maskAlphaChanged(float maskAlpha);
    void panChanged(QVector2D pan);
private:
    int generatorID;
    float squareInPixels;       // uniform for pixel size of a lattice square (aka zoom)
    QVector4D mask;             // uniform for mask shape (leftmost x, topmost y, width, height)
    float maskAlpha;            // uniform for mask alpha
    QVector2D pan;              // uniform for lattice pan (managed in shader)
    bool flagDebug = false;
};
