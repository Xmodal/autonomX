// Copyright 2020, Atsushi Masumori & Xmodal
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

#include "GeneratorHistoryData.h"

GeneratorHistoryData::GeneratorHistoryData(int sizeMax) : data(sizeMax) {
    this->sizeMax = sizeMax;
}

void GeneratorHistoryData::addHistory(double value) {
    if(!firstFrame) {
        // reset change status if not on first frame
        clearChanges();
    } else {
        // set appropriate status for first frame
        firstFrame = false;
        startIndexChanged = true;
        sizeMaxChanged = true;
    }

    // update history buffer
    if(sizeValid == sizeMax) {
        // buffer is full

        // index to write to is the previous start index
        int index = startIndex;
        // write new value
        data[index] = value;
        // increment start index
        startIndex = (startIndex + 1) % sizeMax;
        // set changed status
        dataChanged = true;
        startIndexChanged = true;
    } else {
        // buffer is not full yet

        // index to write to is at start index offset by valid size
        int index = (startIndex + sizeValid) % sizeMax;
        // write new value
        data[index] = value;
        // increment valid size
        sizeValid++;
        // set changed status
        dataChanged = true;
        sizeValidChanged = true;
    }
}

bool GeneratorHistoryData::checkDataChanged() {
    return dataChanged;
}

bool GeneratorHistoryData::checkStartIndexChanged() {
    return startIndexChanged;
}

bool GeneratorHistoryData::checkSizeMaxChanged() {
    return sizeMaxChanged;
}

bool GeneratorHistoryData::checkSizeValidChanged() {
    return sizeValidChanged;
}

void GeneratorHistoryData::clearChanges() {
    dataChanged = startIndexChanged = sizeMaxChanged = sizeValidChanged = false;
}

int GeneratorHistoryData::getStartIndex() {
    return startIndex;
}

int GeneratorHistoryData::getSizeMax() {
    return sizeMax;
}

int GeneratorHistoryData::getSizeValid() {
    return sizeValid;
}

double GeneratorHistoryData::at(int index) {
    return data[index];
}
