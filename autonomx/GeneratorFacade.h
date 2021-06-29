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
#include <QSharedPointer>

#include "Generator.h"

class GeneratorFacade : public QQmlPropertyMap
{
    Q_OBJECT
public:
    GeneratorFacade(Generator *alias);
    ~GeneratorFacade();

    Q_INVOKABLE GeneratorRegionSet* getInputRegionModel();
    Q_INVOKABLE GeneratorRegionSet* getOutputRegionModel();
private:
//    QSharedPointer<GeneratorRegionModel> inputRegionModel;
//    QSharedPointer<GeneratorRegionModel> outputRegionModel;
    bool flagDebug = false;

    Generator* generator;
public slots:
    void updateValueFromAlias(const QString &key, const QVariant &value);

    Q_INVOKABLE void initialize();
    Q_INVOKABLE void resetParameters();
    Q_INVOKABLE void resetRegions();
signals:
    // this is fired when a value is changed from a call to the updateValueFromAlias function, in contrast to valueChanged, which fires when the underlying QQmlPropertyMap is updated from QML
    void valueChangedFromAlias(const QString &key, const QVariant &value);
};
