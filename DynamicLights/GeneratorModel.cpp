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

#include "GeneratorModel.h"
#include <chrono>
#include <QThread>
#include <QVector>
#include <QDebug>

GeneratorModel::GeneratorModel(QSharedPointer<QList<QSharedPointer<Facade>>> generatorFacades) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (GeneratorModel)\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    this->generatorFacades = generatorFacades;

    for(int i = 0; i < generatorFacades.get()->count(); i++) {
        // update model every time something is changed
        connect(generatorFacades.get()->at(i).get(), &Facade::valueChangedFromAlias, this, [=](const QString &key, const QVariant &value) {
            QVector<int> roles;
            bool unrecognized = false;

            int role = roleNames().key(key.toUtf8());
            if (role == 0) unrecognized = true;         // is this is a fully error-proof check?
            else roles = { role };

            // TODO: this is dumb duplicated code that could be streamlined by instead using the roleNames() hashMap. rewritten above.
//            if(key == "name") {
//                roles = {NameRole};
//            } else if (key == "type") {
//                roles = {TypeRole};
//            } else if (key == "description") {
//                roles = {DescriptionRole};
//            } else if (key == "outputMonitor") {
//                roles = {OutputMonitorRole};
//            } else if (key == "outputMonitorHistory") {
//                roles = {OutputMonitorHistoryRole};
//            } else if (key == "outputMonitorHistoryStartIndex") {
//                roles = {OutputMonitorHistoryStartIndexRole};
//            } else if (key == "outputMonitorHistorySizeMax") {
//                roles = {OutputMonitorHistorySizeMaxRole};
//            } else if (key == "outputMonitorHistorySizeValid") {
//                roles = {OutputMonitorHistorySizeValidRole};
//            } else {
//                unrecognized = true;
//            }

            if(flagDebug) {
                std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                );

                QByteArray keyArray = key.toLocal8Bit();
                char* keyBuffer = keyArray.data();

                QByteArray valueArray = value.toString().toLocal8Bit();
                char* valueBuffer = valueArray.data();

                qDebug() << "lambda (" << keyBuffer << "):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\t value = " << valueBuffer << (unrecognized ? " (unrecognized)" : "");
            }

            if(!unrecognized) {
                emit dataChanged(index(i), index(i), roles);
            }
        });
    }
}

GeneratorModel::~GeneratorModel() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (GeneratorModel)\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
}

int GeneratorModel::rowCount(const QModelIndex& parent) const {
    return generatorFacades.get()->size();
}

int GeneratorModel::columnCount(const QModelIndex& parent) const {
  return 1;
}

QVariant GeneratorModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid())
        return QVariant();

    if(index.column() == 0 && index.row() >= 0 && index.row() < generatorFacades.get()->size()) {

        // is a truthy-value check necessary here? or is the key always guaranteed to exist?
        return generatorFacades.get()->at(index.row())->value(roleNames().value(role));

        // TODO: this is dumb duplicated code that could be streamlined by instead using the roleNames() hashMap. rewritten above.
//        switch(role) {
//            case NameRole : {
//                return generatorFacades[index.row()]->value("name");
//                break;
//            }
//            case TypeRole : {
//                return generatorFacades[index.row()]->value("type");
//                break;
//            }
//            case DescriptionRole : {
//                return generatorFacades[index.row()]->value("desctiption");
//                break;
//            }
//            case OutputMonitorRole : {
//                qDebug() << role;
//                return generatorFacades[index.row()]->value("outputMonitor");
//                break;
//            }
//            case OutputMonitorHistoryRole : {
//                return generatorFacades[index.row()]->value("outputMonitorHistory");
//                break;
//            }
//            case OutputMonitorHistoryStartIndexRole : {
//                qDebug() << role;
//                return generatorFacades[index.row()]->value("outputMonitorHistoryStartIndex");
//                break;
//            }
//            case OutputMonitorHistorySizeMaxRole : {
//                return generatorFacades[index.row()]->value("outputMonitorHistorySizeMax");
//                break;
//            }
//            case OutputMonitorHistorySizeValidRole : {
//                return generatorFacades[index.row()]->value("outputMonitorHistorySizeValid");
//                break;
//            }
//        }
    }

    return QVariant();
}

QHash<int, QByteArray> GeneratorModel::roleNames() const {
    QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[DescriptionRole] = "description";
        roles[OutputMonitorRole] = "outputMonitor";
        roles[OutputMonitorHistoryRole] = "outputMonitorHistory";
        roles[OutputMonitorHistoryStartIndexRole] = "outputMonitorHistoryStartIndex";
        roles[OutputMonitorHistorySizeMaxRole] = "outputMonitorHistorySizeMax";
        roles[OutputMonitorHistorySizeValidRole] = "outputMonitorHistorySizeValid";
        return roles;
}

Facade * GeneratorModel::at(int index) {
    if (index < 0) return nullptr;
    return generatorFacades.get()->at(index).get();
}
