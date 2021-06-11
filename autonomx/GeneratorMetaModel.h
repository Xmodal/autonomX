#pragma once

#include <QObject>
#include <QAbstractListModel>

#include "GeneratorMeta.h"

class GeneratorMetaModel : public QAbstractListModel
{
    Q_OBJECT
public:
    void insertAtEnd(QString type);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE GeneratorMeta * at(QString type) const;
private:
    void beginInsertAtEnd();
    void endInsertAtEnd();

    QMap<QString, GeneratorMeta*> metaMap;
};
