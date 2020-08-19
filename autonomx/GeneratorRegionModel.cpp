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

#include <QDebug>
#include <QThread>
#include <QQmlEngine>

#include "GeneratorRegionModel.h"

GeneratorRegionModel::GeneratorRegionModel() {
    // set ownership of the GeneratorFacade to C++ so that QML doesn't try to take over garbage collection duties, resulting in a double free
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (GeneratorRegionModel)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();

    }

    regionList.append((QSharedPointer<GeneratorRegion>) new GeneratorRegion(QRect(0, 0, 10, 10), 0.0));
}

GeneratorRegionModel::~GeneratorRegionModel() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (GeneratorRegionModel)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();

    }
}

int GeneratorRegionModel::rowCount(const QModelIndex& parent) const {
    return regionList.size();
}

int GeneratorRegionModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant GeneratorRegionModel::data(const QModelIndex &index, int role) const {
    if(flagDebug) {
        qDebug() << "data (GeneratorRegionModel)";
    }

    if(index.isValid()) {
        // check if the index is valid
        if(index.column() == 0 && index.row() >= 0 && index.row() < regionList.size()) {
            // check if the key exists in the hash map
            if(roleMap.contains(role))
                return regionList.at(index.row())->property(roleMap.value(role));
        }
    }

    if(flagDebug) {
        qDebug() << "data (GeneratorRegionModel) failed to find valid entry";
    }

    return QVariant();
}

bool GeneratorRegionModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(flagDebug) {
        qDebug() << "setData (GeneratorRegionModel) value: " << value;
    }

    if(index.isValid()) {
        // check if the index is valid
        if(index.column() == 0 && index.row() >= 0 && index.row() < regionList.size()) {
            // check if the key exists in the hash map
            if(roleMap.contains(role)) {
                // ctash here at setProperty
                regionList.at(index.row())->setProperty(roleMap.value(role), value);
                emit writeRegion(value, role, index.row());
                if(flagDebug) {
                    qDebug() << "setData (GeneratorRegionModel): signal emitted";
                }
                return true;
            }
        }
    }

    if(flagDebug) {
        qDebug() << "setData (GeneratorRegionModel) failed to find valid entry";
    }

    return false;

}

QHash<int, QByteArray> GeneratorRegionModel::roleNames() const {
    return roleMap;
}

GeneratorRegion* GeneratorRegionModel::at(int index) {
    if (index < 0) return nullptr;
    return regionList.at(index).data();
}
