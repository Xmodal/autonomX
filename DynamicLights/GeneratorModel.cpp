#include "GeneratorModel.h"
#include <chrono>
#include <QThread>
#include <QVector>
#include <QDebug>

GeneratorModel::GeneratorModel(QList<QSharedPointer<Facade>> generatorFacades) {
    this->generatorFacades = generatorFacades;

    for(int i = 0; i < generatorFacades.count(); i++) {
        // update model every time something is changed
        connect(generatorFacades[i].get(), &Facade::valueChangedFromAlias, this, [=](const QString &key, const QVariant &value) {
            QVector<int> roles;
            bool unrecognized = false;

            if(key == "name") {
                roles = {NameRole};
            } else if (key == "type") {
                roles = {TypeRole};
            } else if (key == "description") {
                roles = {DescriptionRole};
            } else if (key == "outputMonitor") {
                roles = {OutputMonitorRole};
            } else {
                unrecognized = true;
            }

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

int GeneratorModel::rowCount(const QModelIndex& parent) const {
    return generatorFacades.size();
}

int GeneratorModel::columnCount(const QModelIndex& parent) const {
  return 1;
}

QVariant GeneratorModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid())
        return QVariant();

    if(index.column() == 0 && index.row() >= 0 && index.row() < generatorFacades.size()) {
        switch(role) {
            case NameRole : {
                return generatorFacades[index.row()]->value("name");
                break;
            }
            case TypeRole : {
                return generatorFacades[index.row()]->value("type");
                break;
            }
            case DescriptionRole : {
                return generatorFacades[index.row()]->value("desctiption");
                break;
            }
            case OutputMonitorRole : {
                return generatorFacades[index.row()]->value("outputMonitor");
                break;
            }
        }
    }

    return QVariant();
}

QHash<int, QByteArray> GeneratorModel::roleNames() const {
    QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[DescriptionRole] = "description";
        roles[OutputMonitorRole] = "outputMonitor";
        return roles;
}

Facade * GeneratorModel::at(int index) {
    if (index < 0) return nullptr;
    return generatorFacades[index].get();
}
