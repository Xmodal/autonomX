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

#include "Facade.h"

#include <chrono>
#include <QThread>
#include <QDebug>
#include <QMetaProperty>
#include <QQmlEngine>

Facade::Facade(QObject *alias) : QQmlPropertyMap(this, nullptr) {
    // set ownership of the Facade to C++ so that there is no duplicate deletion attempt when the app quits
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (Facade)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();

    }

    const QMetaObject *metaObject = alias->metaObject();

    for (int i = 0; i < metaObject->propertyCount(); ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *name = metaProperty.name();
        QVariant value = alias->property(name);
        // add key-value pair to facade
        insert(name, value);

        if(flagDebug) {
            qDebug() << "added property (Facade): " << name;
        }
    }
}

Facade::~Facade() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (Facade)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
}

void Facade::updateValueFromAlias(const QString &key, const QVariant &value) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        QByteArray keyArray = key.toLocal8Bit();
        char* keyBuffer = keyArray.data();

        QString valueString = value.toString();

        qDebug() << "updateValueFromAlias (" << keyBuffer << ") (in Facade):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tvalue: " << valueString;
    }

    // filter for unchanging updates
    QVariant currentValue = this->value(key);
    if(currentValue != value) {
        insert(key, value);
        emit valueChangedFromAlias(key, value);
    }
}
