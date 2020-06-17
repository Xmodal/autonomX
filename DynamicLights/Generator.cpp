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

#include "Generator.h"

Generator::Generator(QObject *parent) : QObject(parent)
{

}

void Generator::writeInput(double value, int index) {
    input[index] = value;
}

double Generator::readOutput(int index) {
    return output[index];
}

int Generator::getInputSize() {
    return input.size();
}

int Generator::getOutputSize() {
    return output.size();
}

QString Generator::getName() {
    return name;
}

QString Generator::getType() {
    return type;
}

QString Generator::getDescription() {
    return description;
}

double Generator::getOutputMonitor() {
    return outputMonitor;
}

void Generator::writeName(QString string) {
    name = string;
    emit nameChanged(name);
}

void Generator::writeType(QString string) {
    type = string;
    emit typeChanged(type);
}

void Generator::writeDescription(QString string) {
    description = string;
    emit descriptionChanged(string);
}

void Generator::writeOutputMonitor(double value) {
    outputMonitor = value;
    emit outputMonitorChanged(outputMonitor);
}

