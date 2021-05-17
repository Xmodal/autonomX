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
#include <QFile>

#include "OscEngine.h"
#include "ComputeEngine.h"
#include "Generator.h"
#include "GeneratorField.h"
#include "GeneratorFacade.h"
#include "GeneratorModel.h"
#include "GeneratorRegion.h"
#include "GeneratorRegionModel.h"
#include "GeneratorLattice.h"
#include "GeneratorMetaModel.h"
#include "SpikingNet.h"
#include "AppModel.h"
#include "CursorOverrider.h"

// only include AppNap code if platform is macOS
#ifdef Q_OS_MAC
#include "AppNap.h"
#endif

bool flagDebug = false;

int main(int argc, char *argv[]) {
    // disable App Nap on macOS to prevent compute / osc thread from stalling
    #ifdef Q_OS_MAC
    disableAppNap();
    #endif

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


    // generator meta model
    // (tests in here because defining it in AppModel causes crashes fsr)
    // TODO: move this in AppModel?
    GeneratorMetaModel generatorMetaModel;
    generatorMetaModel.insertAtEnd("SNN");


    // expose custom types to QML and Qt's meta type system
    qmlRegisterUncreatableType<Generator>("ca.hexagram.xmodal.autonomx", 1, 0, "Generator", "Cannot instanciate Generator.");
    qmlRegisterUncreatableType<GeneratorFacade>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorFacade", "Cannot instanciate GeneratorFacade.");
    qmlRegisterUncreatableType<GeneratorRegion>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorRegion", "Cannot instanciate GeneratorRegion.");
    qmlRegisterUncreatableType<GeneratorRegionModel>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorRegionModel", "Cannot instanciate GeneratorRegionModel.");
    qmlRegisterUncreatableType<GeneratorMeta>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorMeta", "Cannot instanciate GeneratorMeta.");
    qmlRegisterType<GeneratorLattice>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorLattice");
    qRegisterMetaType<QSharedPointer<Generator>>();


    // register generators to QML engine
    // TODO: find a way to not have to declare these in main.cpp;
    // maybe on type registry initialization?
    qmlRegisterUncreatableType<SpikingNet>("ca.hexagram.xmodal.autonomx", 1, 0, "SpikingNet", "Cannot instanciate SpikingNet.");
    qmlRegisterUncreatableMetaObject(NeuronTypeNS::staticMetaObject, "ca.hexagram.xmodal.autonomx", 1, 0, "NeuronType", "Cannot instanciate NeuronType.");


    // create initial generators
    AppModel::getInstance().createGenerator("spiking_net");
    // AppModel::getInstance().createGenerator("wolfram");

    QQmlApplicationEngine qmlEngine;

    // add cursor overrider object
    // this allows any part of the application to instantaneously alter the mouse cursor shape
    // (mainly used for slide-dragging interactions)
    CursorOverrider co(&app);
    qmlEngine.rootContext()->setContextObject(&co);

    // Pass C++ objects to QML.
    qmlEngine.rootContext()->setContextProperty("appModel", &AppModel::getInstance());
    qmlEngine.rootContext()->setContextProperty("generatorModel", AppModel::getInstance().getGeneratorModel().data());
    qmlEngine.rootContext()->setContextProperty("generatorMetaModel", &generatorMetaModel);
    qmlEngine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (qmlEngine.rootObjects().isEmpty())
        return -1;

    // end compute and osc threads on application exit
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [](){
        AppModel::getInstance().getComputeThread()->exit();
        AppModel::getInstance().getOscThread()->exit();
    });

    // start QML application and return error code
    return app.exec();
}
