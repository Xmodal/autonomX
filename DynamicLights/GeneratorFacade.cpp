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

#include "GeneratorFacade.h"

#include <iostream>
#include <chrono>
#include <QThread>

void GeneratorFacade::updateValueInterceptor(const QString &key, const QVariant &value) {

    std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );

    QByteArray keyArray = key.toLocal8Bit();
    char* keyBuffer = keyArray.data();

    QByteArray valueArray = value.toString().toLocal8Bit();
    char* valueBuffer = valueArray.data();

    this->updateValue(key, value);
    std::cout << "updateValue (" << keyBuffer << "):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\t value = " << valueBuffer << std::endl;
}
