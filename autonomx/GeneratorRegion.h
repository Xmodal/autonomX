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
#include <QRect>
#include <QHash>

class GeneratorRegion : public QObject {
    Q_OBJECT
    Q_PROPERTY(QRect rect READ getRect WRITE writeRect NOTIFY rectChanged)
    Q_PROPERTY(double intensity READ getIntensity WRITE writeIntensity NOTIFY intensityChanged)
    Q_PROPERTY(int type READ getType WRITE writeType NOTIFY typeChanged)
public:
    GeneratorRegion();
    GeneratorRegion(QRect rect, double intensity, int type);
    GeneratorRegion(const GeneratorRegion& region);

    // enum used by GeneratorRegionModel and GeneratorRegionSet
    enum GeneratorRegionRoles {
        RectRole = Qt::UserRole + 1,
        IntensityRole,
        TypeRole
    };

    // role map used by eneratorRegionModel and GeneratorRegionSet
    static inline const QHash<int, QByteArray> roleMap = {
        {RectRole, "rect"},
        {IntensityRole, "intensity"},
        {TypeRole, "type"}
    };

    QRect getRect() const;
    double getIntensity() const;
    int getType() const;

    void writeRect(QRect rect);
    void writeIntensity(double intensity);
    void writeType(int type);
private:
    QRect rect;
    double intensity;
    int type; // region type. 0 = input, 1 = output
signals:
    void rectChanged(QRect rect);
    void intensityChanged(double intensity);
    void typeChanged(int type);
    void valueChanged(const QString &key, const QVariant &value);
};
