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

#include <QSettings>

// this is a wrapper for QSettings to make it exposable in QML
// all it does is make the value method invokable;
// you still can't add settings from within QML. that has to (and should) be done in main.cpp
// or anywhere in the backend, really
class Settings : public QSettings
{
    Q_OBJECT
public:
    Settings(QObject *parent = 0);
    virtual ~Settings();

    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue);
};
