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

#include <QDebug>

#include "AppModel.h"
#include "SpikingNet.h"
#include "WolframCA.h"

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
        qDebug() << "constructor (AppModel): initializing generatorMetaModel";
    }

    // init data (type registry, sort of)
    generatorMetaModel = QSharedPointer<GeneratorMetaModel>(new GeneratorMetaModel());
    generatorMetaModel->insertAtEnd("SpikingNet");

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

    // connect compute engine data output to osc engine
    connect(computeEngine.data(), &ComputeEngine::sendOscData, oscEngine.data(), &OscEngine::sendOscData, Qt::QueuedConnection);

    // connect osc engine data reception to compute engine
    connect(oscEngine.data(), &OscEngine::receiveOscData, computeEngine.data(), &ComputeEngine::receiveOscData, Qt::QueuedConnection);

    // connect signal for adding a generator to the data structures
    connect(this, &AppModel::addGenerator, computeEngine.data(), &ComputeEngine::addGenerator);

    // connect signal for removing a generator from the data structures
    connect(this, &AppModel::removeGenerator, computeEngine.data(), &ComputeEngine::removeGenerator);

    // connect signal for starting osc processing
    connect(this, &AppModel::startGeneratorOsc, oscEngine.data(), &OscEngine::startGeneratorOsc);

    // connect signal for stopping osc processing
    connect(this, &AppModel::stopGeneratorOsc, oscEngine.data(), &OscEngine::stopGeneratorOsc);

    if(flagDebug) {
        qDebug() << "constructor (AppModel): starting computeThread";
    }

    computeThread->start(QThread::TimeCriticalPriority);
    computeEngine->moveToThread(computeThread.data());

    // we need to move all Generator instances to computeThread
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
        (*it)->moveToThread(computeThread.data());
    }

    if(flagDebug) {
        qDebug() << "constructor (AppModel): starting oscThread";
    }

    oscThread->start(QThread::TimeCriticalPriority);
    oscEngine->moveToThread(oscThread.data());

    computeEngine->start();
}

QSharedPointer<QThread> AppModel::getComputeThread() const {
    return computeThread;
}

QSharedPointer<QThread> AppModel::getOscThread() const {
    return oscThread;
}

QSharedPointer<ComputeEngine> AppModel::getComputeEngine() const {
    return computeEngine;
}

QSharedPointer<OscEngine> AppModel::getOscEngine() const {
    return oscEngine;
}

void AppModel::createGenerator(QString type) {
    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): finding a free id";
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

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): creating a new generator";
    }

    // create a new generator
    QSharedPointer<Generator> generator;
    if (type.compare("SpikingNet") == 0) {
        generator = QSharedPointer<Generator>(new SpikingNet(nextID, generatorMetaModel->at("SpikingNet")));
    }

    // move the Generator to computeThread
    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): moving generator and region sets to computeThread";
    }

    generator->moveToThread(computeThread.data());
    generator->getInputRegionSet()->moveToThread(computeThread.data());
    generator->getOutputRegionSet()->moveToThread(computeThread.data());

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): adding generator to data structures later on computeThread";
    }

    // we can't add to the Generator list directly because this could break things on the computeThread.
    // add it later on the computeThread thread once it is safe to do so
    emit addGenerator(generator);

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): starting OSC processing for generator later on oscThread";
    }

    // we setup the osc engine to process data for the generator
    emit startGeneratorOsc(generator);

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): begin insertion at end of generatorModel";
    }

    // tell the generatorModel we are about to insert a generator at the end of the list
    generatorModel->beginInsertAtEnd();

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): creating generatorFacade";
    }

    // create a GeneratorFacade
    QSharedPointer<GeneratorFacade> generatorFacade = QSharedPointer<GeneratorFacade>(new GeneratorFacade(generator.data()));

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): adding generatorFacade to data structures";
    }

    generatorFacadesList->append(generatorFacade);
    generatorFacadesHashMap->insert(generator->getID(), generatorFacade);

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): end insertion at end of generatorModel";
    }

    // tell the generatorModel we are done inserting a generator at the end of the list
    generatorModel->endInsertAtEnd();

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): relinking generatorModel connections";
    }

    // once the list is changed, update the GeneratorModel connections
    generatorModel->relinkConnections();
}

void AppModel::deleteGenerator(int id) {
    if(flagDebug) {
        qDebug() << "deleteGenerator (AppModel): getting generator by id: " << id;
    }
    QSharedPointer<Generator> generator = generatorsHashMap->value(id);

    if(flagDebug) {
        qDebug() << "deleteGenerator (AppModel): removing generator from data structures later on computeThread";
    }

    // we can't delete on the Generator list directly because this could break things on the computeThread.
    // delete it later on the computeThread thread once it is safe to do so
    emit removeGenerator(generator);

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): stopping OSC processing for generator later on oscThread";
    }

    // we ask the osc engine to stop processing data for this generator
    emit stopGeneratorOsc(generator);

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): begin removal at specific id in generatorModel";
    }

    // tell the generatorModel we are about to delete a generator with a specific ID
    generatorModel->beginRemoveAtID(id);

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): removing generatorFacade from data structures";
    }

    // delete the generatorFacade from the list
    for(QList<QSharedPointer<GeneratorFacade>>::iterator it = generatorFacadesList->begin(); it != generatorFacadesList->end(); it++) {
        if(id == (*it)->value("id").toInt()) {
            // erase from the list
            generatorFacadesList->erase(it);
            break;
        }
    }
    // delete the generatorFacade from the hash map
    generatorFacadesHashMap->remove(generator->getID());

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): end removal at specific id in generatorModel";
    }

    // tell the generatorModel we are done deleting a generator with a specific ID
    generatorModel->endRemoveAtID();

    if(flagDebug) {
        qDebug() << "createGenerator (AppModel): relinking generatorModel connections";
    }

    // once the list is changed, update the GeneratorModel connections
    generatorModel->relinkConnections();
}

bool AppModel::validateNewGeneratorName(QString name) {
    bool valid = true;
    for(QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end() && valid; it++) {
        valid = name != it->data()->getName();
    }
    return valid;
}

bool AppModel::loadProject(QString uri)
{
    // parse URI
    QUrl url(uri);
    // create QFile at URI
    QFile loadFile(url.toLocalFile());

    // try to open file
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open save file:" << uri;
        return false;
    }

    // read file
    QByteArray saveData = loadFile.readAll();

    // convert to JSON document
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    // read data
    readJson(loadDoc.object());

    // success!
    return true;
}

bool AppModel::saveProject(QString uri) {
    // parse URI
    QUrl url(uri);
    // create QFile at URL
    QFile saveFile(url.toLocalFile());

    // try to open file
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open save file:" << uri;
        return false;
    }

    // create JSON object
    QJsonObject saveObject;
    // write all data
    writeJson(saveObject);
    // save to file
    saveFile.write(QJsonDocument(saveObject).toJson());

    // success
    return true;
}

QSharedPointer<Generator> AppModel::getGenerator(int id) const {
    return generatorsHashMap->value(id);
}

QSharedPointer<GeneratorFacade> AppModel::getGeneratorFacade(int id) const {
    return generatorFacadesHashMap->value(id);
}

QSharedPointer<GeneratorModel> AppModel::getGeneratorModel() const {
    return generatorModel;
}

QSharedPointer<GeneratorMetaModel> AppModel::getGeneratorMetaModel() const
{
    return generatorMetaModel;
}

void AppModel::readJson(const QJsonObject &json)
{
    qDebug() << json["test"];
}

void AppModel::writeJson(QJsonObject &json) const
{
    json["test"] = "yo :)";
}
