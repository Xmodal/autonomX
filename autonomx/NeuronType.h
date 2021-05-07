// Copyright 2020, Atsushi Masumori & Xmodal
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

// see https://qml.guide/enums-in-qt-qml/
class NeuronTypeClass : public QObject {
    Q_GADGET
public:
    enum Enum {
        SpikingNeuron,
        SpikingNeuronRandomized,
        ResonatorNeuron,
        ResonatorNeuronRandomized,
        ChatteringNeuron,
        InhibitoryNeuron,
        InhibitoryNeuronRandomized,
        ExcitatoryNeuron,
        ExcitatoryNeuronRandomized
    };

    Q_ENUM(Enum);

    explicit NeuronTypeClass();
};

// redefine class member as global enum
typedef NeuronTypeClass::Enum NeuronType;
