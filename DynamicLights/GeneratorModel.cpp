#include "GeneratorModel.h"

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
            case roleName : {
                return QVariant(generators[index.row()]->getName());
                break;
            }
            case roleType : {
                return QVariant(generators[index.row()]->getType());
                break;
            }
            case roleDescription : {
                return QVariant(generators[index.row()]->getDescription());
                break;
            }
            case roleOutputMonitor : {
                return QVariant(generators[index.row()]->getOutputMonitor());
                break;
            }
        }
    }

    return QVariant();
}
