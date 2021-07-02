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

#include "OscEngineFacade.h"

OscEngineFacade::OscEngineFacade(QSharedPointer<OscEngine> oscEngine) : QQmlPropertyMap(this, nullptr)
{
    this->oscEngine = oscEngine;

    // set ownership to C++
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // init properties
    const QMetaObject *metaObject = oscEngine->metaObject();

    for (int i = 0; i < metaObject->propertyCount(); ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *name = metaProperty.name();
        QVariant value = oscEngine->property(name);
        // add key-value pair to facade
        insert(name, value);
    }

    // link signals
    QObject::connect(oscEngine.data(), &OscEngine::valueChanged, this, &OscEngineFacade::updateValueFromAlias, Qt::QueuedConnection);
    QObject::connect(this, &OscEngineFacade::valueChanged, oscEngine.data(), &OscEngine::updateValue, Qt::QueuedConnection);
}

void OscEngineFacade::updateValueFromAlias(const QString &key, const QVariant &value) {
    QVariant currentValue = this->value(key);
    if (currentValue != value) {
        insert(key, value);
        emit valueChangedFromAlias(key, value);
    }
}
