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
#include <QQmlPropertyMap>

// the Facade objects requires that the object it aliases emits a valueChanged(const QString &key, const QVariant &value) signal when any of its QProperties is changed. This is expected to behave identically to QQmlPropertyMap's implementation.
// once created, the Facade will have a copy of the QObject's properties, but they are not linked yet. to link them, use:

// connect alias changes to facade
// QObject::connect(alias, &Alias::valueChanged, fadcade, &Facade::updateValueFromAlias, Qt::QueuedConnection);

// connect facade changes to alias
// QObject::connect(facade, &Facade::valueChanged, alias, &Alias::updateValue, Qt::QueuedConnection);

// this unfortunately can't happen in Facade's constructor because QObject doesn't have a valueChanged() signal by default


class Facade : public QQmlPropertyMap
{
    Q_OBJECT
public:
    Facade(QObject *alias);
    ~Facade();
private:
    bool flagDebug = false;
public slots:
    void updateValueFromAlias(const QString &key, const QVariant &value);
signals:
    // this is fired when a value is changed from a call to the updateValueFromAlias function, in contrast to valueChanged, which fires when the underlying QQmlPropertyMap is updated from QML
    void valueChangedFromAlias(const QString &key, const QVariant &value);
};
