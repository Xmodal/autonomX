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
#include <QList>
#include <QHash>
#include <QSharedPointer>
#include <QThread>

#include "ComputeEngine.h"
#include "OscEngine.h"
#include "Generator.h"
#include "GeneratorFacade.h"
#include "GeneratorModel.h"
#include "GeneratorMetaModel.h"

class AppModel : public QObject {
    Q_OBJECT
public:
    static AppModel& getInstance() {            // get singleton instance
        static AppModel instance;
        return instance;
    }
    // these return shared pointers to avoid threading issues related to the deletion of these objects.
    // be careful! whenever a Generator or GeneratorFacade is accessed through one of these, the reference must go out of scope when the generator is removed so that the objects are actually deleted.
    QSharedPointer<QThread>             getComputeThread() const;
    QSharedPointer<QThread>             getOscThread() const;
    QSharedPointer<ComputeEngine>       getComputeEngine() const;
    QSharedPointer<OscEngine>           getOscEngine() const;
    QSharedPointer<Generator>           getGenerator(int id) const;
    QSharedPointer<GeneratorFacade>     getGeneratorFacade(int id) const;
    QSharedPointer<GeneratorModel>      getGeneratorModel() const;
    QSharedPointer<GeneratorMetaModel>  getGeneratorMetaModel() const;

    Q_INVOKABLE void    createGenerator(QString type);
    Q_INVOKABLE void    deleteGenerator(int id);
    Q_INVOKABLE bool    validateNewGeneratorName(QString name);
private:
    AppModel();                                 // prevent instanciation
    AppModel(AppModel const&) = delete;         // prevent copy
    void operator=(AppModel const&) = delete;   // prevent assignment

    // data (lists)
    QSharedPointer<QList<QSharedPointer<Generator>>> generatorsList;
    QSharedPointer<QList<QSharedPointer<GeneratorFacade>>> generatorFacadesList;

    // data (hash maps)
    QSharedPointer<QHash<int, QSharedPointer<Generator>>> generatorsHashMap;
    QSharedPointer<QHash<int, QSharedPointer<GeneratorFacade>>> generatorFacadesHashMap;

    // data (unique elements)
    QSharedPointer<GeneratorModel> generatorModel;
    QSharedPointer<GeneratorMetaModel> generatorMetaModel;

    // engines
    QSharedPointer<ComputeEngine> computeEngine;
    QSharedPointer<OscEngine> oscEngine;

    // threads
    QSharedPointer<QThread> computeThread;
    QSharedPointer<QThread> oscThread;

    // utility variables
    bool flagDebug = false;
signals:
    // connects to ComputeEngine::addGenerator for safe addition of generators to data structures
    void addGenerator(QSharedPointer<Generator> generator);

    // connects to ComputeEngine::removeGenerator for safe removal of generators from data structures
    void removeGenerator(QSharedPointer<Generator> generator);

    // connects to OscEngine::startGeneratorOsc for safe start of osc processing
    void startGeneratorOsc(QSharedPointer<Generator> generator);

    // connects to OscEngine::stopGeneratorOsc for safe stop of osc processing
    void stopGeneratorOsc(QSharedPointer<Generator> generator);
};
