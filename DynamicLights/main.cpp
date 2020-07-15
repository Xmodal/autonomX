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

#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QList>
#include <QSharedPointer>
#include <QFontDatabase>
#include <QQmlPropertyMap>

#include "OscEngine.h"
#include "ComputeEngine.h"
#include "Generator.h"
#include "GeneratorModel.h"
#include "SpikingNet.h"
#include "Facade.h"

// only include AppNap code if platform is macOS
#ifdef Q_OS_MAC
#include "AppNap.h"
#endif

bool flagDebug = false;

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // load fonts in the project database
    QDir fontDir{":/assets/fonts"};
    for (auto file : fontDir.entryList(QDir::Files)) {
        if (QFontDatabase::addApplicationFont(":/assets/fonts/" + file) == -1)
            qDebug() << "Failed to load font " << file;
    }

    qDebug() << "Built against Qt" << QT_VERSION_STR;
    qDebug() << "Using Qt" << QLibraryInfo::version() << "at runtime";


    // make Generator virtual class recognizable to QML
    // this line is apparently necessary for the QML engine to receive Generator pointers
    // and retrieve a class instance's exposed properties by model index through said pointer
    qmlRegisterUncreatableType<Generator>("ca.hexagram.xmodal.dynamiclight", 1, 0, "Generator", "Cannot instanciate Generator.");
    qmlRegisterUncreatableType<Facade>("ca.hexagram.xmodal.dynamiclight", 1, 0, "Facade", "Cannot instanciate Facade.");
    qmlRegisterUncreatableType<SpikingNet>("ca.hexagram.xmodal.dynamiclight", 1, 0, "SpikingNet", "Cannot instanciate SpikingNet.");
    qmlRegisterUncreatableType<NeuronType>("ca.hexagram.xmodal.dynamiclight", 1, 0, "NeuronType", "Cannot instanciate NeuronType.");

    // create generator list
    QSharedPointer<Generator> spikingNet = QSharedPointer<Generator>(new SpikingNet(0));
    QSharedPointer<QList<QSharedPointer<Generator>>> generators = QSharedPointer<QList<QSharedPointer<Generator>>>(new QList<QSharedPointer<Generator>>());
    generators.get()->append(spikingNet);


    // create generator facade list
    QSharedPointer<QList<QSharedPointer<Facade>>> generatorFacades = QSharedPointer<QList<QSharedPointer<Facade>>>(new QList<QSharedPointer<Facade>>());

    // create facades and link them
    for(QList<QSharedPointer<Generator>>::iterator it = generators.get()->begin(); it != generators.get()->end(); it++) {
        QSharedPointer<Generator> generator = *it;
        QSharedPointer<Facade> generatorFacade = QSharedPointer<Facade>(new Facade(generator.get()));

        // connect generator changes to facade
        QObject::connect(generator.data(), &Generator::valueChanged, generatorFacade.data(), &Facade::updateValueFromAlias, Qt::QueuedConnection);
        // connect facade changes to generator
        QObject::connect(generatorFacade.get(), &Facade::valueChanged, generator.get(), &Generator::updateValue, Qt::QueuedConnection);
        // add the newly constructed facade to the list
        generatorFacades.get()->append(generatorFacade);
    }

    // create generator model
    GeneratorModel generatorModel(generatorFacades);

    // create compute thread
    QSharedPointer<QThread> computeThread = QSharedPointer<QThread>(new QThread());
    computeThread->start(QThread::TimeCriticalPriority);

    // create compute engine
    ComputeEngine computeEngine(generators);

    // move compute engine to compute thread
    computeEngine.moveToThread(computeThread.get());

    // move generators to compute thread
    for(QList<QSharedPointer<Generator>>::iterator it = generators.get()->begin(); it != generators.get()->end(); it++) {
        (*it)->moveToThread(computeThread.get());
    }

    // create osc thread
    QSharedPointer<QThread> oscThread = QSharedPointer<QThread>(new QThread());
    oscThread->start(QThread::TimeCriticalPriority);

    // create osc engine
    OscEngine oscEngine;

    // move osc engine to compute thread
    oscEngine.moveToThread(oscThread.get());

    // connect compute engine to osc engine
    QObject::connect(&computeEngine, &ComputeEngine::sendOscData, &oscEngine, &OscEngine::sendOscData);

    QObject::connect(&computeEngine, &ComputeEngine::createOscReceiver, &oscEngine, &OscEngine::createOscReceiver);
    QObject::connect(&computeEngine, &ComputeEngine::updateOscReceiver, &oscEngine, &OscEngine::updateOscReceiver);
    QObject::connect(&computeEngine, &ComputeEngine::deleteOscReceiver, &oscEngine, &OscEngine::deleteOscReceiver);

    QObject::connect(&computeEngine, &ComputeEngine::createOscSender, &oscEngine, &OscEngine::createOscSender);
    QObject::connect(&computeEngine, &ComputeEngine::updateOscSender, &oscEngine, &OscEngine::updateOscSender);
    QObject::connect(&computeEngine, &ComputeEngine::deleteOscSender, &oscEngine, &OscEngine::deleteOscSender);

    // connect osc engine to compute engine
    QObject::connect(&oscEngine, &OscEngine::recieveOscData, &computeEngine, &ComputeEngine::recieveOscData);

    // start compute engine
    computeEngine.start();

    QQmlApplicationEngine qmlEngine;

    // Pass C++ objects to QML.
    qmlEngine.rootContext()->setContextProperty("generatorModel", &generatorModel);
    qmlEngine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (qmlEngine.rootObjects().isEmpty())
        return -1;

    // create debug connections (optional)
    if(flagDebug) {
        for(QList<QSharedPointer<Generator>>::iterator it = generators.get()->begin(); it != generators.get()->end(); it++) {
            QObject::connect((*it).data(), &Generator::valueChanged, &computeEngine, [=](const QString &key, const QVariant &value) {
                std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                );

                QByteArray keyArray = key.toLocal8Bit();
                char* keyBuffer = keyArray.data();

                QString valueString = value.toString();

                qDebug() << "valueChanged (" << keyBuffer << ") (caught from Generator):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tvalue: " << valueString;
            });
        }

        for(QList<QSharedPointer<Facade>>::iterator it = generatorFacades.get()->begin(); it != generatorFacades.get()->end(); it++) {
            QObject::connect((*it).data(), &Facade::valueChanged, &computeEngine, [=](const QString &key, const QVariant &value) {
                std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                );

                QByteArray keyArray = key.toLocal8Bit();
                char* keyBuffer = keyArray.data();

                QString valueString = value.toString();

                qDebug() << "valueChanged (" << keyBuffer << ") (caught from Facade):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tvalue: " << valueString;
            });

            QObject::connect((*it).data(), &Facade::valueChangedFromAlias, &computeEngine, [=](const QString &key, const QVariant &value) {
                std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                );

                QByteArray keyArray = key.toLocal8Bit();
                char* keyBuffer = keyArray.data();

                QString valueString = value.toString();

                qDebug() << "valueChangedFromAlias (" << keyBuffer << ") (caught from Facade):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tvalue: " << valueString;
            });
        }
    }

    // end compute and osc threads on application exit
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [computeThread, oscThread](){
        computeThread->exit();
        oscThread->exit();
    });

    // disable App Nap on macOS to prevent compute / osc thread from stalling
    #ifdef Q_OS_MAC
    disableAppNap();
    #endif

    // start QML application and return error code
    return app.exec();
}
