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

#include <chrono>
#include <QThread>
#include <QDebug>
#include <QMetaProperty>
#include <QQmlEngine>

#include "GeneratorFacade.h"

GeneratorFacade::GeneratorFacade(Generator *generator) : QQmlPropertyMap(this, nullptr) {
    // set ownership of the GeneratorFacade to C++ so that there is no duplicate deletion attempt when the app quits
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (GeneratorFacade)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();

    }

    // init properties
    const QMetaObject *metaObject = generator->metaObject();

    for (int i = 0; i < metaObject->propertyCount(); ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *name = metaProperty.name();
        QVariant value = generator->property(name);
        // add key-value pair to facade
        insert(name, value);

        if(flagDebug) {
            qDebug() << "added property (GeneratorFacade): " << name;
        }
    }

    // init region models
    inputRegionsModel = QSharedPointer<GeneratorRegionModel>(new GeneratorRegionModel);
    outputRegionsModel = QSharedPointer<GeneratorRegionModel>(new GeneratorRegionModel);

    // connect generator changes to facade
    QObject::connect(generator, &Generator::valueChanged, this, &GeneratorFacade::updateValueFromAlias, Qt::QueuedConnection);
    // connect facade changes to generator
    QObject::connect(this, &GeneratorFacade::valueChanged, generator, &Generator::updateValue, Qt::QueuedConnection);

    // connect facade region model changes to region set
    QObject::connect(inputRegionsModel.data(), &GeneratorRegionModel::addRegion, generator->getInputRegionSet(), &GeneratorRegionSet::addRegion);
    QObject::connect(outputRegionsModel.data(), &GeneratorRegionModel::addRegion, generator->getOutputRegionSet(), &GeneratorRegionSet::addRegion);

    QObject::connect(inputRegionsModel.data(), &GeneratorRegionModel::deleteRegion, generator->getInputRegionSet(), &GeneratorRegionSet::deleteRegion);
    QObject::connect(outputRegionsModel.data(), &GeneratorRegionModel::deleteRegion, generator->getOutputRegionSet(), &GeneratorRegionSet::deleteRegion);

    QObject::connect(inputRegionsModel.data(), &GeneratorRegionModel::writeRegion, generator->getInputRegionSet(), &GeneratorRegionSet::writeRegion);
    QObject::connect(outputRegionsModel.data(), &GeneratorRegionModel::writeRegion, generator->getOutputRegionSet(), &GeneratorRegionSet::writeRegion);
}

GeneratorFacade::~GeneratorFacade() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (GeneratorFacade)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
}

void GeneratorFacade::updateValueFromAlias(const QString &key, const QVariant &value) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        QByteArray keyArray = key.toLocal8Bit();
        char* keyBuffer = keyArray.data();

        QString valueString = value.toString();

        qDebug() << "updateValueFromAlias (" << keyBuffer << ") (in GeneratorFacade):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tvalue: " << valueString;
    }

    // filter for unchanging updates
    QVariant currentValue = this->value(key);
    if(currentValue != value) {
        insert(key, value);
        emit valueChangedFromAlias(key, value);
    }
}

GeneratorRegionModel* GeneratorFacade::getInputRegionModel() {
    return inputRegionsModel.data();
}

GeneratorRegionModel* GeneratorFacade::getOutputRegionModel() {
    return outputRegionsModel.data();
}
