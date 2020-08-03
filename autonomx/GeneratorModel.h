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

#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QModelIndex>
#include <QSharedPointer>
#include <QList>
#include <QHash>
#include <QMetaObject>
#include <GeneratorFacade.h>

class GeneratorModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum GeneratorRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        IDRole,
        DescriptionRole,
        OutputMonitorRole,
        OutputMonitorHistoryRole,
        OutputMonitorHistoryStartIndexRole,
        OutputMonitorHistorySizeMaxRole,
        OutputMonitorHistorySizeValidRole
    };

    GeneratorModel(QSharedPointer<QList<QSharedPointer<GeneratorFacade>>> generatorFacadesList, QSharedPointer<QHash<int, QSharedPointer<GeneratorFacade>>> generatorFacadesHashMap);
    ~GeneratorModel();

    // GeneratorModel is constructed from a GeneratorFacade list which is identical to the one found in AppModel. This is not a copy, this is the same memory location.
    // whenever AppModel adds or removes a GeneratorFacade, all connections between each GeneratorFacade's valueChangedFromAlias and the GeneratorModel's updateValueFromAlias must be reset, because the order of the list might have changed, which breaks the indexing system used by the dataChanged signal eventually emitted by updateValueFromAlias.

    // iterates over all members of the GeneratorFacade list and creates connections for them.
    void createAliasConnections();
    // deletes all connections from GeneratorFacade. WARNING: the way this is implemented, this will delete all connections to the GeneratorModel. Keep this in mind, otherwise things might break in the future.
    void deleteAliasConnections();
    // this simply deletes then creates the connections. This is called by AppModel whenever the list changes.
    void relinkAliasConnections();

    // these two functions must be called before and after a generator is added at the end of the list so that the model is updated properly
    void beginInsertAtEnd();
    void endInsertAtEnd();

    // these two functions must be called before and after a generator is deleted at the end of the list so that the model is updated properly
    void beginRemoveAtID(int id);
    void endRemoveAtID();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void populate();
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE GeneratorFacade * at(int index);

public slots:
    void updateValueFromAlias(const QString &key, const QVariant &value, int modelIndex);

private:
    QList<QMetaObject::Connection> connections;
    QSharedPointer<QList<QSharedPointer<GeneratorFacade>>> generatorFacadesList;
    QSharedPointer<QHash<int, QSharedPointer<GeneratorFacade>>> generatorFacadesHashMap;
    bool flagDebug = false;
    const QHash<int, QByteArray> roleMap = {
        {NameRole, "name"},
        {TypeRole, "type"},
        {IDRole, "id"},
        {DescriptionRole, "description"},
        {OutputMonitorRole, "outputMonitor"},
        {OutputMonitorHistoryRole, "outputMonitorHistory"},
        {OutputMonitorHistoryStartIndexRole, "outputMonitorHistoryStartIndex"},
        {OutputMonitorHistorySizeMaxRole, "outputMonitorHistorySizeMax"},
        {OutputMonitorHistorySizeValidRole,"outputMonitorHistorySizeValid"}
    };
};
