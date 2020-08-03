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

#include "AppModel.h"
#include "SpikingNet.h"
#include <QDebug>

AppModel::AppModel() {
    if(flagDebug) {
        qDebug() << "constructor (AppModel): initializing lists";
    }

    // init data (lists)
    generatorsList = QSharedPointer<QList<QSharedPointer<Generator>>>(new QList<QSharedPointer<Generator>>());
    generatorFacadesList = QSharedPointer<QList<QSharedPointer<GeneratorFacade>>>(new QList<QSharedPointer<GeneratorFacade>>());

    if(flagDebug) {
        qDebug() << "constructor (AppModel): initializing hash maps";
    }

    // init data (hash maps)
    generatorsHashMap = QSharedPointer<QHash<int, QSharedPointer<Generator>>>(new QHash<int, QSharedPointer<Generator>>());
    generatorFacadesHashMap = QSharedPointer<QHash<int, QSharedPointer<GeneratorFacade>>>(new QHash<int, QSharedPointer<GeneratorFacade>>());

    if(flagDebug) {
        qDebug() << "constructor (AppModel): initializing generatorModel";
    }

    // init data (unique elements)
    generatorModel = QSharedPointer<GeneratorModel>(new GeneratorModel(generatorFacadesList, generatorFacadesHashMap));

    if(flagDebug) {
        qDebug() << "constructor (AppModel): initializing engines";
    }

    // init engines
    computeEngine = QSharedPointer<ComputeEngine>(new ComputeEngine(generatorsList, generatorsHashMap));
    oscEngine = QSharedPointer<OscEngine>(new OscEngine());

    if(flagDebug) {
        qDebug() << "constructor (AppModel): initializing threads";
    }

    // init threads
    computeThread = QSharedPointer<QThread>(new QThread());
    oscThread = QSharedPointer<QThread>(new QThread());

    if(flagDebug) {
        qDebug() << "constructor (AppModel): connecting engines";
    }

    // connect compute engine setting changes to osc engine
    connect(computeEngine.data(), &ComputeEngine::createOscReceiver, oscEngine.data(), &OscEngine::createOscReceiver, Qt::QueuedConnection);
    connect(computeEngine.data(), &ComputeEngine::deleteOscReceiver, oscEngine.data(), &OscEngine::deleteOscReceiver, Qt::QueuedConnection);

    connect(computeEngine.data(), &ComputeEngine::createOscSender, oscEngine.data(), &OscEngine::createOscSender, Qt::QueuedConnection);
    connect(computeEngine.data(), &ComputeEngine::deleteOscSender, oscEngine.data(), &OscEngine::deleteOscSender, Qt::QueuedConnection);

    // connect compute engine data output to osc engine
    connect(computeEngine.data(), &ComputeEngine::sendOscData, oscEngine.data(), &OscEngine::sendOscData, Qt::QueuedConnection);

    // connect osc engine data reception to compute engine
    connect(oscEngine.data(), &OscEngine::recieveOscData, computeEngine.data(), &ComputeEngine::recieveOscData, Qt::QueuedConnection);
}

void AppModel::start() {
    if(flagDebug) {
        qDebug() << "start (AppModel): starting computeThread";
    }

    computeThread->start(QThread::TimeCriticalPriority);
    computeEngine->moveToThread(computeThread.data());

    // we need to move all Generator instances to computeThread
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
        (*it)->moveToThread(computeThread.data());
    }

    if(flagDebug) {
        qDebug() << "start (AppModel): starting oscThread";
    }

    oscThread->start(QThread::TimeCriticalPriority);
    oscEngine->moveToThread(oscThread.data());

    computeEngine->start();

    started = true;
}

QThread* AppModel::getComputeThread() const {
    return computeThread.data();
}

QThread* AppModel::getOscThread() const {
    return oscThread.data();
}

ComputeEngine*  AppModel::getComputeEngine() const {
    return computeEngine.data();
}

OscEngine* AppModel::getOscEngine() const {
    return oscEngine.data();
}

void AppModel::createGenerator() {
    if(flagDebug) {
        qDebug() << "createGenerator (AppModel)";
    }
    // find a free ID for the new generator
    // create a temporary list of taken IDs
    // we do this instead of using generatorsList because we will want to sort it, and this is thread-unsafe since computeEngine might be iterating generators it at any time
    QList<int> takenIDs;
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
        int takenID = (*it)->getID();
        takenIDs.append(takenID);
    }
    // we sort the taken IDs
    std::sort(takenIDs.begin(), takenIDs.end());
    // we search for a free ID by looking through the sorted taken IDs, incrementing the next ID if it is already taken
    int nextID = 0;
    for(QList<int>::iterator it = takenIDs.begin(); it != takenIDs.end(); it++) {
        int takenID = (*it);
        if(nextID == takenID) {
            nextID++;
        }
    }

    // create a new generator
    QSharedPointer<Generator> generator = QSharedPointer<Generator>(new SpikingNet(nextID));

    // move the Generator to computeThread if it is started
    if(started) {
        generator->moveToThread(computeThread.data());
    }

    // we can't add to the Generator list directly because this could break things on the computeThread.
    // add it later on the computeThread thread once it is safe to do so
    // this also takes care of creating the appropriate osc sender and receiver
    emit computeEngine->addGenerator(generator);

    // tell the generatorModel we are about to insert a generator at the end of the list
    generatorModel->beginInsertAtEnd();

    // create a GeneratorFacade and add it to the list
    QSharedPointer<GeneratorFacade> generatorFacade = QSharedPointer<GeneratorFacade>(new GeneratorFacade(generator.data()));
    generatorFacadesList->append(generatorFacade);

    // tell the generatorModel we are done inserting a generator at the end of the list
    generatorModel->endInsertAtEnd();

    // once the list is changed, update the GeneratorModel connections
    generatorModel->relinkAliasConnections();
}

void AppModel::deleteGenerator(int id) {
    if(flagDebug) {
        qDebug() << "deleteGenerator (AppModel)";
    }
    // we can't delete on the Generator list directly because this could break things on the computeThread.
    // delete it later on the computeThread thread once it is safe to do so
    // this also takes care of deleting the appropriate osc sender and receiver
    emit computeEngine->deleteGenerator(id);

    // tell the generatorModel we are about to delete a generator with a specific ID
    generatorModel->beginRemoveAtID(id);

    // delete the GeneratorFacade from the list
    for(QList<QSharedPointer<GeneratorFacade>>::iterator it = generatorFacadesList->begin(); it != generatorFacadesList->end(); it++) {
        if(id == (*it)->value("id").toInt()) {
            // erase from the list
            generatorFacadesList->erase(it);
            break;
        }
    }

    // tell the generatorModel we are done deleting a generator with a specific ID
    generatorModel->endRemoveAtID();

    // once the list is changed, update the GeneratorModel connections
    generatorModel->relinkAliasConnections();
}

bool AppModel::validateNewGeneratorName(QString name) {
    bool valid = true;
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end() && valid; it++) {
        valid = name != it->data()->getName();
    }
    return valid;
}

Generator* AppModel::getGenerator(int id) const {
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
        if(id == (*it)->getID()) {
            return (*it).data();
        }
     }
    return nullptr;
}

GeneratorFacade* AppModel::getGeneratorFacade(int id) const {
    for(QList<QSharedPointer<GeneratorFacade>>::iterator it = generatorFacadesList->begin(); it != generatorFacadesList->end(); it++) {
        if(id == (*it)->value("id").toInt()) {
            return (*it).data();
        }
     }
    return nullptr;
}

GeneratorModel* AppModel::getGeneratorModel() const {
    return generatorModel.data();
}
