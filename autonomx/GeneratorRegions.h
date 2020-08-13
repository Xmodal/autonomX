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
#include <QList>
#include <QRect>

class GeneratorRegions : public QObject{
    Q_OBJECT
public:
    GeneratorRegions();

    QRect getRect(int index);
    float getIntensity(int index);
private:
    QList<QRect> rectList;
    QList<float> intensityList;
public slots:
    void createRegion();
    void deleteRegion(int index);

    void writeRect(QRect rect, int index);
    void writeIntensity(float intensity, int index);
};

