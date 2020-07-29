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

AppModel::AppModel() {
    // init data (lists)
    generators = QSharedPointer<QList<QSharedPointer<Generator>>>(new QList<QSharedPointer<Generator>>());
    generatorFacades = QSharedPointer<QList<QSharedPointer<GeneratorFacade>>>(new QList<QSharedPointer<GeneratorFacade>>());

    // init data (unique elements)
    QSharedPointer<GeneratorModel> generatorModel = QSharedPointer<GeneratorModel>(new GeneratorModel(generatorFacades));

    // init engines
    computeEngine = QSharedPointer<ComputeEngine>(new ComputeEngine(generators));
    oscEngine = QSharedPointer<OscEngine>(new OscEngine());

    // init threads
    computeThread = QSharedPointer<QThread>(new QThread());
    oscThread = QSharedPointer<QThread>(new QThread());
}

void AppModel::start() {
    computeThread->start(QThread::TimeCriticalPriority);
    computeEngine->moveToThread(computeThread.data());

    // we need to move all Generator instances to computeThread
    for(QList<QSharedPointer<Generator>>::iterator it = generators.get()->begin(); it != generators.get()->end(); it++) {
        (*it)->moveToThread(computeThread.data());
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

void AppModel::createGenerator() {
    // find a free ID for the new generator
    // create a temporary list of taken IDs
    // we do this instead of using generatorsList because we will want to sort it, and this is thread-unsafe since computeEngine might be iterating generators it at any time
    QList<int> takenIDs;
    for(QList<QSharedPointer<Generator>>::iterator it = generators->begin(); it != generators->end(); it++) {
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

    // we can only create an OscSender / OscReceiver pair once the Generator exists because we have to setup some connections.
    // this means we have to do this after the pervious operation is done.

    // create a GeneratorFacade and add it to the list
    QSharedPointer<GeneratorFacade> generatorFacade = QSharedPointer<GeneratorFacade>(new GeneratorFacade(generator.data()));
    generatorFacades->push_back(generatorFacade);
    // once the list is changed, update the GeneratorModel connections
    generatorModel->relinkAliasConnections();
}

void AppModel::deleteGenerator(int id) {
    // we can't delete on the Generator list directly because this could break things on the computeThread.
    // delete it later on the computeThread thread once it is safe to do so

    // we can delete an OscSender / OscReceiver pair anytime as long as we update OscEngine to not kill itself if an invalid generator id is used (which would happen if the matching OscSender / OscReceiver were deleted before the Generator was deleted, leaving enough time for a ComputeEngine cycle to finish)
    // it's actually much simpler to just handle in ComputeEngine, in the slot dedicated to Generator deletion, much like how we will manage OscSender / OscReceiver creation.

    // once the list is changed, update the GeneratorModel connections
    generatorModel->relinkAliasConnections();
}

bool AppModel::validateNewGeneratorName(QString name) {
    bool valid = true;
    for(QList<QSharedPointer<Generator>>::iterator it = generators->begin(); it != generators->end() && valid; it++) {
        valid = name != it->data()->getName();
    }
    return valid;
}

Generator* AppModel::getGenerator(int id) const {
    for(QList<QSharedPointer<Generator>>::iterator it = generators->begin(); it != generators->end(); it++) {
        if(id == (*it)->getID()) {
            return (*it).data();
        }
     }
    return nullptr;
}

GeneratorFacade* AppModel::getGeneratorFacade(int id) const {
    for(QList<QSharedPointer<GeneratorFacade>>::iterator it = generatorFacades->begin(); it != generatorFacades->end(); it++) {
        if(id == (*it)->value("id").toInt()) {
            return (*it).data();
        }
     }
    return nullptr;
}

GeneratorModel* AppModel::getGeneratorModel() const {
    return generatorModel.data();
}
