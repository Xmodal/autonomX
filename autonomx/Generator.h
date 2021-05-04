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
#include <QString>
#include <QVariant>
#include <QVector>
#include <QSharedPointer>
#include <QMutex>
#include <vector>

#include "GeneratorRegionSet.h"

class Generator : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ getType NOTIFY typeChanged)
    Q_PROPERTY(int id READ getID)
    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString userNotes READ getUserNotes WRITE writeUserNotes NOTIFY userNotesChanged)
    Q_PROPERTY(double historyLatest READ getHistoryLatest NOTIFY historyLatestChanged)
    Q_PROPERTY(bool historyRefresher READ getHistoryRefresher NOTIFY historyRefresherChanged)

    Q_PROPERTY(int oscInputPort READ getOscInputPort WRITE writeOscInputPort NOTIFY oscInputPortChanged)
    Q_PROPERTY(QString oscInputAddress READ getOscInputAddress WRITE writeOscInputAddress NOTIFY oscInputAddressChanged)

    Q_PROPERTY(int oscOutputPort READ getOscOutputPort WRITE writeOscOutputPort NOTIFY oscOutputPortChanged)
    Q_PROPERTY(QString oscOutputAddressHost READ getOscOutputAddressHost WRITE writeOscOutputAddressHost NOTIFY oscOutputAddressHostChanged)
    Q_PROPERTY(QString oscOutputAddressTarget READ getOscOutputAddressTarget WRITE writeOscOutputAddressTarget NOTIFY oscOutputAddressTargetChanged)

    Q_PROPERTY(int latticeWidth READ getLatticeWidth WRITE writeLatticeWidth NOTIFY latticeWidthChanged)
    Q_PROPERTY(int latticeHeight READ getLatticeHeight WRITE writeLatticeHeight NOTIFY latticeHeightChanged)
public:
    // enum used by GeneratorModel
    enum GeneratorRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        IDRole,
        DescriptionRole,
        UserNotesRole,
        HistoryLatestRole,
        HistoryRefresherRole,
    };

    // role map used by GeneratorModel
    static inline const QHash<int, QByteArray> roleMap = {
        {NameRole, "name"},
        {TypeRole, "type"},
        {IDRole, "id"},
        {DescriptionRole, "description"},
        {UserNotesRole, "userNotes"},
        {HistoryLatestRole, "historyLatest"},
        {HistoryRefresherRole, "historyRefresher"}
    };

    Generator(int id, QString name, QString type, QString description);
    ~Generator();

    // method implemented by the derived class that computes the output
    // this assumes the input values have already been set on the lattice by ComputeEngine before this call.
    // this also doesn't take care of retrieving output values on the lattice, as this is handled by ComputeEngine after this call.
    virtual void computeIteration(double deltaTime) = 0;

    // this is the mutex used by writeLatticeData. GeneratorLatticeRenderer will lock it while using *latticeData in its render method.
    void lockLatticeDataMutex();
    void unlockLatticeDataMutex();

    // methods to read properties
    QString getName();
    QString getType();
    int getID();
    QString getDescription();
    QString getUserNotes();
    double getHistoryLatest();
    bool getHistoryRefresher();

    int getOscInputPort();
    QString getOscInputAddress();

    int getOscOutputPort();
    QString getOscOutputAddressHost();
    QString getOscOutputAddressTarget();

    int getLatticeWidth();
    int getLatticeHeight();

    // methods to write properties
    void writeType(QString type);
    void writeUserNotes(QString userNotes);
    void writeHistoryLatest(double historyLatest);
    void flipHistoryRefresher(); // this inverts the historyRefresher's value

    void writeOscInputPort(int oscInputPort);
    void writeOscInputAddress(QString oscInputAddress);

    void writeOscOutputPort(int oscOutputPort);
    void writeOscOutputAddressHost(QString oscOutputAddressHost);
    void writeOscOutputAddressTarget(QString oscOutputAddressTarget);

    // these only take care of doing the signaling
    void writeLatticeWidth(int latticeWidth);
    void writeLatticeHeight(int latticeHeight);

    // these are implemented by the derived class and take care of any memory reallocation needed to change the size of the algorithm.
    // this is called by writeLatticeWidth / writeLatticeHeight. this must set the variable to the passed value.
//    virtual void writeLatticeWidthDelegate(int latticeWidth) = 0;
//    virtual void writeLatticeHeightDelegate(int latticeHeight) = 0;

    // these are implemented by the derived class and allow reading / writing to the lattice.
    // this is called by applyInputRegion / applyOutputRegion
    virtual double getLatticeValue(int x, int y) = 0;
    virtual void writeLatticeValue(int x, int y, double value) = 0;

    // reinitialize generator
    virtual void reset() = 0;
    virtual void initialize() = 0;
    virtual void resetParameters() = 0;

    // this updates the lattice / region from the corresponding region / lattice. this is called before / after the call to computeIteration from ComputeEngine
    void applyInputRegion();
    void applyOutputRegion();

    GeneratorRegionSet* getInputRegionSet();
    GeneratorRegionSet* getOutputRegionSet();
protected:
    int latticeWidth = 20;                      // lattice width
    int latticeHeight = 20;                     // lattice height
private:
    int id;                                     // generator id, generated automatically by ComputeEngine in constructor

    QString name;                               // generator name, assigned by user
    QString type;                               // generator type, fixed
    QString description;                        // generator description, fixed
    QString userNotes;                          // user notes, modifiable
    double historyLatest = 0;                   // latest value for the history graph
    bool historyRefresher = false;              // bool that flips every time history latest is refreshed. this is an ugly workaround to prevent Qt from ignoring updates of historyLatest where the value doesn't change.

    int oscInputPort = 6668;                    // generator osc input port, assigned by user
    QString oscInputAddress = "/input";         // generator osc input address, assigned by user (this is an osc destination)

    int oscOutputPort = 6669;                   // generator osc output port, assigned by user
    QString oscOutputAddressHost = "127.0.0.1"; // generator osc output address for host, assigned by user (this is an ip)
    QString oscOutputAddressTarget = "/output"; // generator osc output address for target, assigned by user (this is an osc destination)

    bool flagDebug = false;                     // enables debug

    QMutex latticeDataMutex;                    // mutex used by writeLatticeData

    QSharedPointer<GeneratorRegionSet> inputRegionSet;
    QSharedPointer<GeneratorRegionSet> outputRegionSet;
public slots:
    // common slot allowing to update any property. allows the Facade class to work properly
    // (for connection from QQmlPropertyMap's valueChanged signal)
    void updateValue(const QString &key, const QVariant &value);

    // slot used by GeneratorLatticeCommunicator to get the lattice data.
    //
    // latticeData is a pointer to a pointer which designates the block of memory data is written to.
    // preAllocatedSize represents the amount of allocated memory at *latticeData before the function call
    // postAllocatedSize represents the amouunt of allocated memory at *latticeData after the function call
    //
    // why do allocatedWidth and allocatedHeight exist? because the size of the lattice may change, meaning that **latticeData mismatches the lattice's actual size.
    // in that case, this function will delete *latticeData, and reallocate *latticeData to a new memory block that matches the true size.
    // both are passed by pointer so that GeneratorLatticeRenderer can keep track of the new size after this method is executed.
    // why not just use a getter before the method call? because we want this to be atomic; things could get changed between the getter call and the method call.
    //
    // this method must never be executed while GeneratorLatticeRenderer's render method is using **latticeData. it uses latticeDataMutex to prevent this.
    // if GeneratorLatticeRenderer's render has already locked latticeDataMutex, we schedule the method call to happen at a later time so that the ComputeEngine does not lock
    // we need a mechanism to prevent duplicate requests that would happen if the method is unable to complete before the next render frame however...
    // this is done by emitting writeLatticeDataCompleted once done, and only allowing GeneratorLatticeRenderer to emit writeLatticeData signals if the previous one did complete.
    //
    // the first call to this function is done with latticeData as a new float pointer pointer that is not null, and with allocatedWidth and allocatedHeight as 0. the memory for the inner pointer will be allocated automatically
    //
    // the connection from GeneratorLatticeCommunicator's requestLatticeData signal to Generator's writeLatticeData slot is created from GeneratorLatticeCommunicator
    //
    // latticeData is written to by treating the memory as a flattened 2D double array
    //
    // the indexing scheme goes as follows:
    //
    //    latticeData[indexWidth, indexHeight] = flattenedData[indexWidth + indexHeight * latticeHeight]
    //
    // equivalent to (where / is integer round down division):
    //
    //    flattenedData[index] = latticeData[index % latticeWidth, index / latticeWidth]
    //
    void writeLatticeData(float** latticeData, int* allocatedWidth, int* allocatedHeight);
signals:
    // common signal used alongside all other property change signals. allows the Facade class to work properly
    // (for connection to QQmlPropertyMap's updateValue slot)
    void valueChanged(const QString &key, const QVariant &value);

    // usual signals for property changes
    void nameChanged(QString name);
    void typeChanged(QString type);
    void descriptionChanged(QString description);
    void userNotesChanged(QString userNotes);
    void historyLatestChanged(double historyLatest);
    void historyRefresherChanged(bool historyRefresher);

    void oscInputPortChanged(int oscInputPort);
    void oscInputAddressChanged(QString oscInputAddress);

    void oscOutputPortChanged(int oscOutputPort);
    void oscOutputAddressHostChanged(QString oscOutputAddressHost);
    void oscOutputAddressTargetChanged(QString oscOutputAddressTarget);

    void latticeWidthChanged(int latticeWidth);
    void latticeHeightChanged(int latticeHeight);

    // tells the GeneratorLatticeCommunicator that the previously created writeLatticeData request is completed, and that a new one can be started at the end of the current render frame.
    //
    // the connections from Generator's writeLatticeDataCompleted signal to GeneratorLatticeCommunicator's requestLatticeDataCompleted slot is created from GeneratorLatticeCommunicator
    void writeLatticeDataCompleted();
};
