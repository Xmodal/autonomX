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

#include <QThread>
#include <QTimer>
#include <QElapsedTimer>
#include <QList>
#include <QSharedPointer>
#include "Generator.h"


class ComputeEngine : public QThread
{
private:
    QList<QSharedPointer<Generator>> generators;
    QElapsedTimer elapsedTimer;
    double frequency = 80;
    bool firstFrame = true;
    bool flagDebug = true;
public:
    ComputeEngine(QList<QSharedPointer<Generator>> generators);
    void run() override;
};
