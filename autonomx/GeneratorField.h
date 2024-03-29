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
#include <QVariant>
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QMetaType>


// used for introspection (or is that really what it's for...?)
struct GeneratorField {
    Q_GADGET
    Q_PROPERTY(QString propName MEMBER propName)
    Q_PROPERTY(QString label MEMBER label)
    Q_PROPERTY(QString type MEMBER type)
    Q_PROPERTY(bool restartOnEdit MEMBER restartOnEdit)
    Q_PROPERTY(bool flag MEMBER flag)

    Q_PROPERTY(QVariant defaultValue MEMBER defaultValue)
    Q_PROPERTY(bool flagDefaultValue MEMBER flagDefaultValue)

    Q_PROPERTY(QString placeholder MEMBER placeholder)
    Q_PROPERTY(QVariant min MEMBER min)
    Q_PROPERTY(QVariant max MEMBER max)
    Q_PROPERTY(QVariant updateLag MEMBER updateLag)
    Q_PROPERTY(QString enumName MEMBER enumName)
public:
    GeneratorField() {}

    // All Of THem .
    GeneratorField(
            QString propName,
            QString label,
            QString type,
            bool restartOnEdit,
            bool flag,

            QVariant defaultValue,
            bool flagDefaultValue,

            QString placeholder,
            QVariant min,
            QVariant max,
            QVariant updateLag,
            QString enumName)

        : propName(propName),
          label(label),
          type(type),
          restartOnEdit(restartOnEdit),
          flag(flag),

          defaultValue(defaultValue),
          flagDefaultValue(flagDefaultValue),

          placeholder(placeholder),
          min(min),
          max(max),
          updateLag(updateLag),
          enumName(enumName)
    {}

    GeneratorField(QVariantMap fieldProps) {
        propName = fieldProps["propName"].toString();
        label = fieldProps["label"].toString();
        type = fieldProps["type"].toString();
        restartOnEdit = fieldProps["restartOnEdit"].toBool();
        flag = fieldProps["flag"].toBool();

        defaultValue = fieldProps["default"];
        flagDefaultValue = fieldProps["flagDefault"].toBool();

        placeholder = fieldProps["placeholder"].toString();
        min = fieldProps["min"];
        max = fieldProps["max"];
        updateLag = fieldProps["updateLag"];
        enumName = fieldProps["enumName"].toString();
    }

    QString propName;
    QString label;
    QString type;
    bool restartOnEdit = false;
    bool flag = false;

    QVariant defaultValue;
    bool flagDefaultValue;

    // field-specific
    QString placeholder = "";
    QVariant min = 0;
    QVariant max = 1;
    QVariant updateLag = 0;
    QString enumName;
};
Q_DECLARE_METATYPE(GeneratorField)

// declare hash map type to Qt meta system
// to make the code Better Anmd Ncier :)
typedef QVector<GeneratorField> SubRack;
Q_DECLARE_METATYPE(SubRack)
