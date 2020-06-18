#include "GeneratorModel.h"
#include <iostream>

GeneratorModel::GeneratorModel(QList<QSharedPointer<Generator>> generators) {
    this->generators = generators;
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
};
