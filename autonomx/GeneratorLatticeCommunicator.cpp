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

#include "GeneratorLatticeCommunicator.h"

GeneratorLatticeCommunicator::GeneratorLatticeCommunicator() {}

void GeneratorLatticeCommunicator::updateGenerator(Generator *generator) {
    if(this->generator == nullptr) {
        // no previous connections to delete
    } else {
        // previous connections must be deleted
        disconnect(connectionRequestLatticeData);
        disconnect(connectionRequestLatticeDataCompleted);
    }
    this->generator = generator;

    connectionRequestLatticeData = connect(this, &GeneratorLatticeCommunicator::requestLatticeData, generator, &Generator::writeLatticeData);
    connectionRequestLatticeDataCompleted = connect(generator, &Generator::writeLatticeDataCompleted, this, &GeneratorLatticeCommunicator::requestLatticeDataCompleted);
}

void GeneratorLatticeCommunicator::writeLatticeData(double** latticeData, int* allocatedWidth, int* allocatedHeight) {
    currentRequestDone = false;
    emit requestLatticeData(latticeData, allocatedWidth, allocatedHeight);
}

void GeneratorLatticeCommunicator::requestLatticeDataCompleted() {
    currentRequestDone = true;
    firstRequestDone = true;
}

bool GeneratorLatticeCommunicator::isCurrentRequestDone() {
    return currentRequestDone;
}

bool GeneratorLatticeCommunicator::isFirstRequestDone() {
    return firstRequestDone;
}
