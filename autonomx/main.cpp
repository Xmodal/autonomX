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
#include "GeneratorFacade.h"
#include "GeneratorModel.h"
#include "GeneratorLattice.h"
#include "SpikingNet.h"

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

    qDebug() << "GUI id = " << QThread::currentThreadId();


    // make Generator virtual class recognizable to QML
    // this line is apparently necessary for the QML engine to receive Generator pointers
    // and retrieve a class instance's exposed properties by model index through said pointer
    qmlRegisterUncreatableType<Generator>("ca.hexagram.xmodal.autonomx", 1, 0, "Generator", "Cannot instanciate Generator.");
    qmlRegisterUncreatableType<GeneratorFacade>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorFacade", "Cannot instanciate GeneratorFacade.");
    qmlRegisterUncreatableType<SpikingNet>("ca.hexagram.xmodal.autonomx", 1, 0, "SpikingNet", "Cannot instanciate SpikingNet.");
    qmlRegisterUncreatableType<NeuronType>("ca.hexagram.xmodal.autonomx", 1, 0, "NeuronType", "Cannot instanciate NeuronType.");
    qmlRegisterType<GeneratorLattice>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorLattice");

    // create generator list
    QSharedPointer<Generator> spikingNet = QSharedPointer<Generator>(new SpikingNet(0));
    QSharedPointer<QList<QSharedPointer<Generator>>> generators = QSharedPointer<QList<QSharedPointer<Generator>>>(new QList<QSharedPointer<Generator>>());
    generators.get()->append(spikingNet);

    // create generator facade list
    QSharedPointer<QList<QSharedPointer<GeneratorFacade>>> generatorGeneratorFacades = QSharedPointer<QList<QSharedPointer<GeneratorFacade>>>(new QList<QSharedPointer<GeneratorFacade>>());

    // create facades and link them
    for(QList<QSharedPointer<Generator>>::iterator it = generators.get()->begin(); it != generators.get()->end(); it++) {
        QSharedPointer<Generator> generator = *it;
        QSharedPointer<GeneratorFacade> generatorGeneratorFacade = QSharedPointer<GeneratorFacade>(new GeneratorFacade(generator.get()));

        // add the newly constructed facade to the list
        generatorGeneratorFacades.get()->append(generatorGeneratorFacade);
    }

    // create generator model
    GeneratorModel generatorModel(generatorGeneratorFacades);

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

    // TODO: the code below, which takes care of connecting the osc and compute engine together, is a bit of a mess. find a way to refactor / rearchitect this into something more elegant.

    // connect compute engine setting changes to osc engine
    QObject::connect(&computeEngine, &ComputeEngine::createOscReceiver, &oscEngine, &OscEngine::createOscReceiver, Qt::QueuedConnection);
    QObject::connect(&computeEngine, &ComputeEngine::deleteOscReceiver, &oscEngine, &OscEngine::deleteOscReceiver, Qt::QueuedConnection);

    QObject::connect(&computeEngine, &ComputeEngine::createOscSender, &oscEngine, &OscEngine::createOscSender, Qt::QueuedConnection);
    QObject::connect(&computeEngine, &ComputeEngine::deleteOscSender, &oscEngine, &OscEngine::deleteOscSender, Qt::QueuedConnection);

    // connect compute engine data output to osc engine
    QObject::connect(&computeEngine, &ComputeEngine::sendOscData, &oscEngine, &OscEngine::sendOscData, Qt::QueuedConnection);

    // connect osc engine data reception to compute engine
    QObject::connect(&oscEngine, &OscEngine::recieveOscData, &computeEngine, &ComputeEngine::recieveOscData, Qt::QueuedConnection);

    // connect generator osc setting changes to osc engine
    for(QList<QSharedPointer<Generator>>::iterator it = generators.get()->begin(); it != generators.get()->end(); it++) {
        // get generator info
        QSharedPointer<Generator> generator = *it;
        int id = generator->getID();
        QString addressReceiver = generator->getOscInputAddress();
        QString addressSenderHost = generator->getOscOutputAddressHost();
        QString addressSenderTarget = generator->getOscOutputAddressTarget();
        int portReceiver = generator->getOscInputPort();
        int portSender = generator->getOscOutputPort();

        // create osc sender and receiver
        emit computeEngine.createOscReceiver(id, addressReceiver, portReceiver);
        emit computeEngine.createOscSender(id, addressSenderHost, addressSenderTarget, portSender);

        // connect input / receiver changes
        QObject::connect(generator.data(), &Generator::oscInputAddressChanged, &oscEngine, [&oscEngine, generator](QString oscInputAddress){
            if(flagDebug) {
                qDebug() << "oscInputAddressChanged (lambda)";
            }
            emit oscEngine.updateOscReceiverAddress(generator->getID(), oscInputAddress);
        });

        QObject::connect(generator.data(), &Generator::oscInputPortChanged, &oscEngine, [&oscEngine, generator](int oscInputPort){
            if(flagDebug) {
                qDebug() << "oscInputPortChanged (lambda)";
            }
            emit oscEngine.updateOscReceiverPort(generator->getID(), oscInputPort);
        });

        // connect output / sender changes
        QObject::connect(generator.data(), &Generator::oscOutputAddressHostChanged, &oscEngine, [&oscEngine, generator](QString oscOutputAddressHost){
            if(flagDebug) {
                qDebug() << "oscOutputAddressHostChanged (lambda)";
            }
            emit oscEngine.updateOscSenderAddressHost(generator->getID(), oscOutputAddressHost);
        });

        QObject::connect(generator.data(), &Generator::oscOutputAddressTargetChanged, &oscEngine, [&oscEngine, generator](QString oscOutputAddressTarget){
            if(flagDebug) {
                qDebug() << "oscOutputAddressTargetChanged (lambda)";
            }
            emit oscEngine.updateOscSenderAddressTarget(generator->getID(), oscOutputAddressTarget);
        });

        QObject::connect(generator.data(), &Generator::oscOutputPortChanged, &oscEngine, [&oscEngine, generator](int oscOutputPort){
            if(flagDebug) {
                qDebug() << "oscOutputAddressTargetChanged (lambda)";
            }
            emit oscEngine.updateOscSenderPort(generator->getID(), oscOutputPort);
        });
    }

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

        for(QList<QSharedPointer<GeneratorFacade>>::iterator it = generatorGeneratorFacades.get()->begin(); it != generatorGeneratorFacades.get()->end(); it++) {
            QObject::connect((*it).data(), &GeneratorFacade::valueChanged, &computeEngine, [=](const QString &key, const QVariant &value) {
                std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                );

                QByteArray keyArray = key.toLocal8Bit();
                char* keyBuffer = keyArray.data();

                QString valueString = value.toString();

                qDebug() << "valueChanged (" << keyBuffer << ") (caught from GeneratorFacade):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tvalue: " << valueString;
            });

            QObject::connect((*it).data(), &GeneratorFacade::valueChangedFromAlias, &computeEngine, [=](const QString &key, const QVariant &value) {
                std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                );

                QByteArray keyArray = key.toLocal8Bit();
                char* keyBuffer = keyArray.data();

                QString valueString = value.toString();

                qDebug() << "valueChangedFromAlias (" << keyBuffer << ") (caught from GeneratorFacade):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tvalue: " << valueString;
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
