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

#include "OscReceiver.h"
#include "OscSender.h"
#include "ComputeEngine.h"
#include "Generator.h"
#include "GeneratorModel.h"
#include "SpikingNet.h"
#include "Facade.h"

// only include AppNap code if platform is macOS
#ifdef Q_OS_MAC
#include "AppNap.h"
#endif

#ifdef Q_OS_MAC
#include <IOKit/pwr_mgt/IOPMLib.h>
#endif

int main(int argc, char *argv[])
{
    #ifdef Q_OS_MAC
    //IOPMAssertionCreateWithName()
    #endif

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    int argumentCount = QCoreApplication::arguments().size();
    QStringList argumentList = QCoreApplication::arguments();
    QTextStream standardOutput(stdout);

    // set OSC paraemters
    QString sendOscHost = "127.0.0.1";
    quint16 sendOscPort = 14444;
    quint16 receiveOscPort = 14444;

    // load fonts in the project database
    QDir fontDir{":/assets/fonts"};
    for (auto file : fontDir.entryList(QDir::Files)) {
        if (QFontDatabase::addApplicationFont(":/assets/fonts/" + file) == -1)
            standardOutput << "Failed to load font " << file << endl;
    }

    if (argumentCount > 1) {
        sendOscHost = argumentList.at(1);
    }
    if (argumentCount > 2) {
        sendOscPort = static_cast<quint16>(argumentList.at(2).toInt());
    }
    if (argumentCount > 3) {
        receiveOscPort = static_cast<quint16>(argumentList.at(3).toInt());
    }
    if (argumentCount == 1) {
        standardOutput << QObject::tr("To specify an OSC send host and port: %1 <sendHost> <sendPort> <receivePort").arg(
                              argumentList.first()) << endl;
    }

    qDebug() << "Built against Qt" << QT_VERSION_STR;
    qDebug() << "Using Qt" << QLibraryInfo::version() << "at runtime";

    standardOutput << QObject::tr("Receive OSC on port %1").arg(receiveOscPort) << endl;
    standardOutput << QObject::tr("Send OSC to %1:%2").arg(sendOscHost).arg(sendOscPort) << endl;

    // create OSC sender and receiver
    OscReceiver oscReceiver(receiveOscPort);
    OscSender oscSender(sendOscHost, sendOscPort);

    // make Generator virtual class recognizable to QML
    // this line is apparently necessary for the QML engine to receive Generator pointers
    // and retrieve a class instance's exposed properties by model index through said pointer
    qmlRegisterUncreatableType<Generator>("ca.hexagram.xmodal.dynamiclight", 1, 0, "Generator", "Cannot instanciate Generator.");
    qmlRegisterUncreatableType<Facade>("ca.hexagram.xmodal.dynamiclight", 1, 0, "Facade", "Cannot instanciate Facade.");
    qmlRegisterUncreatableType<SpikingNet>("ca.hexagram.xmodal.dynamiclight", 1, 0, "SpikingNet", "Cannot instanciate SpikingNet.");
    qmlRegisterUncreatableType<NeuronType>("ca.hexagram.xmodal.dynamiclight", 1, 0, "NeuronType", "Cannot instanciate NeuronType.");

    // create generator list
    QSharedPointer<Generator> spikingNet = QSharedPointer<Generator>(new SpikingNet());
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

    // disable App Nap on macOS to prevent computeThread from stalling
    #ifdef Q_OS_MAC
    disableAppNap();
    #endif

    // start compute engine
    computeEngine.start();

    QQmlApplicationEngine engine;

    // Pass C++ objects to QML.
    engine.rootContext()->setContextProperty("generatorModel", &generatorModel);
    engine.rootContext()->setContextProperty("oscSender", &oscSender);
    engine.rootContext()->setContextProperty("oscReceiver", &oscReceiver);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [computeThread](){
        qDebug() << "about to exit";
        computeThread->exit();
    });

    return app.exec();
}
