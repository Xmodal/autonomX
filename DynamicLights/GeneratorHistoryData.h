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

#pragma once

#include <QObject>
#include <QVector>

class GeneratorHistoryData : public QObject {
    Q_OBJECT
private:
    QVector<double> data;   // circular buffer containing the history of the output monitor
    int startIndex = 0;     // index of the first element in the buffer (historically the oldest element)
    int sizeMax;            // size of the circular buffer
    int sizeValid = 0;      // number of valid entries in the circular buffer (initialized to 0 since the buffer will be empty)
    bool dataChanged = false;
    bool startIndexChanged = false;
    bool sizeMaxChanged = false;
    bool sizeValidChanged = false;
    bool firstFrame = true;
    void clearChanges();
public:
    GeneratorHistoryData(int sizeMax);
    void addHistory(double value);
    bool checkDataChanged();
    bool checkStartIndexChanged();
    bool checkSizeMaxChanged();
    bool checkSizeValidChanged();
    int getStartIndex();
    int getSizeMax();
    int getSizeValid();
    Q_INVOKABLE double at(int index);
};
