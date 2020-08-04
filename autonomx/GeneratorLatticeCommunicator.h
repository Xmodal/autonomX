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

#include "Generator.h"

class GeneratorLatticeCommunicator : public QObject {
    Q_OBJECT
public:
    GeneratorLatticeCommunicator();
    void updateGenerator(Generator* generator);
    void writeLatticeData(float** latticeData, int* allocatedWidth, int* allocatedHeight);

    bool isCurrentRequestDone();
    bool isFirstRequestDone();
private:
    Generator* generator = nullptr;
    bool currentRequestDone = true;
    bool firstRequestDone = false;
    bool flagDebug = false;
    QMetaObject::Connection connectionAllocateInitialLatticeData;
    QMetaObject::Connection connectionWriteLatticeData;
    QMetaObject::Connection connectionWriteLatticeDataCompleted;
signals:
    // these shouldn't be used from the outside. these are emitted from writeLatticeData.
    void allocateInitialLatticeDataHandler(float** latticeData, int* allocatedWidth, int* allocatedHeight);
    void writeLatticeDataHandler(float** latticeData, int* allocatedWidth, int* allocatedHeight);
public slots:
    void requestLatticeDataCompleted();
};
