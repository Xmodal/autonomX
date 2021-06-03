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
    Q_PROPERTY(QRect rect READ getRect WRITE writeSilentRect NOTIFY rectChanged)
    Q_PROPERTY(double intensity READ getIntensity WRITE writeSilentIntensity NOTIFY intensityChanged)
    Q_PROPERTY(int type READ getType WRITE writeSilentType NOTIFY typeChanged)
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

    void readJson(const QJsonObject &json);
    void writeJson(QJsonObject &json) const;

    // these are silent writes. they update the value internally and call their linked propertyChanged(value) signals, but don't call valueChanged(key, value). this means this will not update an object that is mirroring it. this is used to carry updates between mirrored objects without causing an infinite update loop.
    // they should not be used directly and are reserved for object mirroring.
    void writeSilentRect(QRect rect);
    void writeSilentIntensity(double intensity);
    void writeSilentType(int type);

    // these are mirrored writes
    void writeMirroredRect(QRect rect);
    void writeMirroredIntensity(double intensity);
    void writeMirroredType(int type);

    // caveat: this only works because the facade objects are not exposed directly as QObjects, but are instead presented through a model that is able to directly call the silent write function on the associated backend object whenever a change is made. (the backend is free to call the mirrored write method, so there is no issue on that side)
    // if these were exposed as QObjects, changes would not be mirrored because the QProperties are bound to the silent write methods.
    // to generalize this to QObjects, it would be necessary to bind the QProperties to the mirrored write methods and create a setSilentProperty counterpart to QObject's setProperty for the mirroring connections to use.
    // in this case it might be beneficial to build a simple meta-property-like system that has both setSilentProperty and setMirroredProperty methods
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
