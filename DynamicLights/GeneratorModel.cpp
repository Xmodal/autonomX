#include "GeneratorModel.h"
#include <iostream>
#include <chrono>
#include <QThread>

GeneratorModel::GeneratorModel(QList<QSharedPointer<Generator>> generators) {
    this->generators = generators;

    for(int i = 0; i < generators.count(); i++) {
        // update model every time the output monitor is computed
        connect(generators[i].get(), &Generator::outputMonitorChanged, this, [=]() {
            emit dataChanged(index(i), index(i), { OutputMonitorRole });
        });

        // update model when name is changed in GenRack (per-generator action)
        connect(generators[i].get(), &Generator::nameChanged, this, [=]() {
            emit dataChanged(index(i), index(i), { NameRole });
        });

        // these are the only two properties that need to force update the model's data
        // since they are always relayed to a ListView
    }
}

int GeneratorModel::rowCount(const QModelIndex& parent) const {
    return generators.size();
}

int GeneratorModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

QVariant GeneratorModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid())
        return QVariant();

    std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now().time_since_epoch()
    );

    std::cout << "data:\t\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << std::endl;

    if(index.column() == 0 && index.row() >= 0 && index.row() < generators.size()) {
        switch(role) {
            case NameRole : {
                return QVariant(generators[index.row()]->getName());
                break;
            }
            case TypeRole : {
                return QVariant(generators[index.row()]->getType());
                break;
            }
            case DescriptionRole : {
                return QVariant(generators[index.row()]->getDescription());
                break;
            }
            case OutputMonitorRole : {
                return QVariant(generators[index.row()]->getOutputMonitor());
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

Generator * GeneratorModel::at(int index)
{
    if (index < 0) return nullptr;
    return generators[index].get();
}
