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
    QML_ELEMENT
    Q_PROPERTY(int generatorID READ getGeneratorID WRITE writeGeneratorID NOTIFY generatorIDChanged)
public:
    QQuickFramebufferObject::Renderer * createRenderer() const;
    int getGeneratorID();
    void writeGeneratorID(int generatorID);
signals:
    void generatorIDChanged(int generatorID);
private:
    int generatorID;
};
