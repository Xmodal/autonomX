#include <QDebug>

#include "GeneratorMetaModel.h"

void GeneratorMetaModel::beginInsertAtEnd()
{
    int indexAdded = metaMap.size();
    beginInsertRows(QModelIndex(), indexAdded, indexAdded);
}

void GeneratorMetaModel::insertAtEnd(QString type)
{
    beginInsertAtEnd();

    // check if type is already defined
    // TODO

    // create new GeneratorMeta instance
    // and append to meta map
    metaMap.insert(type, new GeneratorMeta(type));
    endInsertAtEnd();
}

void GeneratorMetaModel::endInsertAtEnd()
{
    endInsertRows();
}

int GeneratorMetaModel::rowCount(const QModelIndex &parent) const
{
    return metaMap.size();
}

QVariant GeneratorMetaModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.column() == 0 && index.row() >= 0 && index.row() < metaMap.size()) {
        if (GeneratorMeta::roleMap.contains(role)) {
            // here, we target the metaMap item by key index
            // this will have for effect the automatic sorting of all generator types in alphabetical order
            return metaMap[metaMap.keys()[index.row()]]->property(GeneratorMeta::roleMap[role]);
        }
        else return QVariant();

    }
    else return QVariant();
}

QHash<int, QByteArray> GeneratorMetaModel::roleNames() const
{
    return GeneratorMeta::roleMap;
}

GeneratorMeta * GeneratorMetaModel::at(QString type) const
{
    if (type.compare("") == 0) return nullptr;

    // TODO: target by type (ex. "SNN") instead of index
    // and refactor insertAtEnd to compensate for it as well :-)
    return metaMap[type];
}
