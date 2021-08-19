#pragma once

#include <QObject>
#include <QAbstractListModel>

#include "GeneratorMeta.h"

class GeneratorMetaModel : public QAbstractListModel
{
    Q_OBJECT
public:
    void insertAtEnd(QString type);
    int getGeneratorTypeCount();
    QList<QString> getGeneratorTypes();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    int generatorTypeCount = 0;
    QList<QString> generatorTypes;

    Q_INVOKABLE GeneratorMeta * at(QString type) const;
private:
    void beginInsertAtEnd();
    void endInsertAtEnd();

    QMap<QString, GeneratorMeta*> metaMap;
};
