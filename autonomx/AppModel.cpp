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
#include "GameOfLife.h"

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
    generatorMetaModel->insertAtEnd("WolframCA");
    generatorMetaModel->insertAtEnd("GameOfLife");

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

QSharedPointer<Generator> AppModel::createGenerator(QString type) {
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

    //// Add New Generator Type Here ////
    // create a new generator
    QSharedPointer<Generator> generator;
    if (type.compare("SpikingNet") == 0) {
        generator = QSharedPointer<Generator>(new SpikingNet(nextID, generatorMetaModel->at("SpikingNet")));
    } else if (type.compare("WolframCA") == 0) {
        generator = QSharedPointer<Generator>(new WolframCA(nextID, generatorMetaModel->at("WolframCA")));
    } else if (type.compare("GameOfLife") == 0) {
        generator = QSharedPointer<Generator>(new GameOfLife(nextID, generatorMetaModel->at("GameOfLife")));
    }
    else if (type.compare("GameOfLife") == 0) {
        generator = QSharedPointer<Generator>(new GameOfLife(nextID, generatorMetaModel->at("GameOfLife")));
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

    return generator;
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

    // check validity of file contents;
    // we are reading from a binary MIME type
    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(url.toLocalFile());

    if (type.name().compare("application/octet-stream") != 0) {
        qWarning() << "Couldn't open save file :: MIME type not acceptable.";
        return false;
    }

    // try to open file
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open save file:" << uri;
        return false;
    }

    // read file
    QByteArray saveData = loadFile.readAll();
    // convert to JSON document
    QJsonDocument loadDoc(QCborValue::fromCbor(saveData).toMap().toJsonObject());

    // read data
    if (!readJson(loadDoc.object())) {
        qWarning() << "Could not load project.";
        return false;
    };

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
    if (!writeJson(saveObject)) {
        qWarning() << "Error writing to save file.";
        return false;
    }

    // save to file
    saveFile.write(QCborValue::fromJsonValue(saveObject).toCbor());

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

bool AppModel::readJson(const QJsonObject &json)
{
    // version check
    const QString version = QCoreApplication::applicationVersion();
    if (json["version"].toString().compare(version) != 0) {
        qDebug() << "save data version differs from app version. be careful!";
    }

    // destroy all current generators
    deleteAllGenerators();

    // create generators based on JSON
    QJsonArray generators = json["generators"].toArray();
    for (int i = 0; i < generators.size(); i++) {
        QJsonObject generatorData = generators[i].toObject();

        // create a generator, retrieving the pointer
        QSharedPointer<Generator> generator = createGenerator(generatorData["type"].toString());

        // send save data to Generator function
        generator->readJson(generatorData);
    }

    // success
    return true;
}

bool AppModel::writeJson(QJsonObject &json) const
{
    json["appName"] = QCoreApplication::applicationName();
    json["version"] = QCoreApplication::applicationVersion();
    json["savedAt"] = QJsonValue::fromVariant(QVariant::fromValue(std::time(0)));

    // TODO: global OSC I/O port numbers + send host

    // write generator data
    QJsonArray generatorData;

    for (QSharedPointer<Generator> g : *generatorsList) {
        QJsonObject singleGeneratorData;
        g->writeJson(singleGeneratorData);
        generatorData.append(singleGeneratorData);
    }

    // add to global JSON
    json["generators"] = generatorData;

    // success
    return true;
}

void AppModel::deleteAllGenerators()
{
    for (QList<QSharedPointer<Generator>>::iterator it = generatorsList->begin(); it != generatorsList->end(); it++) {
        deleteGenerator((*it)->getID());
    }
}
