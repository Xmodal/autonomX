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
#include <QSettings>

#include "OscEngine.h"
#include "ComputeEngine.h"
#include "Generator.h"
#include "GeneratorField.h"
#include "GeneratorFacade.h"
#include "GeneratorModel.h"
#include "GeneratorLattice.h"
#include "GeneratorMetaModel.h"
#include "SpikingNet.h"
#include "WolframCA.h"
#include "GameOfLife.h"
#include "AppModel.h"
#include "CursorOverrider.h"
#include "Settings.h"

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

    // constant settings
    const char* applicationName = "autonomX";
    const char* applicationVersion = "0.1.1";
    const char* organizationName = "Xmodal";

    // global settings
    QCoreApplication::setApplicationName(applicationName);
    QCoreApplication::setApplicationVersion(applicationVersion);
    QCoreApplication::setOrganizationName(organizationName);

    Settings* settings = new Settings();
    settings->setValue("global/extensionName", "atnx");
    settings->setValue("global/repoUrl", "https://github.com/Xmodal/autonomX");


    // load fonts in the project database
    QDir fontDir{":/assets/fonts"};
    for (auto file : fontDir.entryList(QDir::Files)) {
        if (QFontDatabase::addApplicationFont(":/assets/fonts/" + file) == -1)
            qDebug() << "Failed to load font " << file;
    }


    qDebug() << "Built against Qt" << QT_VERSION_STR;
    qDebug() << "Using Qt" << QLibraryInfo::version() << "at runtime";


    // expose custom types to QML and Qt's meta type system
    qmlRegisterUncreatableType<Generator>("ca.hexagram.xmodal.autonomx", 1, 0, "Generator", "Cannot instanciate Generator.");
    qmlRegisterUncreatableType<GeneratorFacade>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorFacade", "Cannot instanciate GeneratorFacade.");
    qmlRegisterUncreatableType<GeneratorRegion>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorRegion", "Cannot instanciate GeneratorRegion.");
    qmlRegisterUncreatableType<GeneratorRegionSet>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorRegionSet", "Cannot instanciate GeneratorRegionSet.");
    qmlRegisterUncreatableType<GeneratorMeta>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorMeta", "Cannot instanciate GeneratorMeta.");
    qmlRegisterUncreatableType<OscEngine>("ca.hexagram.xmodal.autonomx", 1, 0, "OscEngine", "Cannot instanciate OscEngine.");
    qmlRegisterType<GeneratorLattice>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorLattice");
    qRegisterMetaType<QSharedPointer<Generator>>();

    //// Register All Generator Types to QML Engine Here ////
    // TODO: find a way to not have to declare these in main.cpp;
    // maybe on type registry initialization?
    qmlRegisterUncreatableType<SpikingNet>("ca.hexagram.xmodal.autonomx", 1, 0, "SpikingNet", "Cannot instanciate SpikingNet.");
    qmlRegisterUncreatableType<WolframCA>("ca.hexagram.xmodal.autonomx", 1, 0, "WolframCA", "Cannot instanciate WolframCA.");
    qmlRegisterUncreatableType<GameOfLife>("ca.hexagram.xmodal.autonomx", 1, 0, "GameOfLife", "Cannot instanciate Game Of Life.");
    qmlRegisterUncreatableMetaObject(NeuronTypeNS::staticMetaObject, "ca.hexagram.xmodal.autonomx", 1, 0, "NeuronType", "Cannot instanciate NeuronType.");
    qmlRegisterUncreatableMetaObject(GOLType::staticMetaObject, "ca.hexagram.xmodal.autonomx", 1, 0, "GOLPatternType", "Cannot instanciate GOLType.");


    // create initial generator
    AppModel::getInstance().createGenerator("SpikingNet");

    QQmlApplicationEngine qmlEngine;

    // add cursor overrider object
    // this allows any part of the application to instantaneously alter the mouse cursor shape
    // (mainly used for slide-dragging interactions)
    CursorOverrider co(&app);
    qmlEngine.rootContext()->setContextObject(&co);

    // Pass C++ objects to QML.
    qmlEngine.rootContext()->setContextProperty("appModel", &AppModel::getInstance());
    qmlEngine.rootContext()->setContextProperty("generatorModel", AppModel::getInstance().getGeneratorModel().data());
    qmlEngine.rootContext()->setContextProperty("generatorMetaModel", AppModel::getInstance().getGeneratorMetaModel().data());
    qmlEngine.rootContext()->setContextProperty("oscEngine", AppModel::getInstance().getOscEngineFacade().data());
    qmlEngine.rootContext()->setContextProperty("settings", settings);
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
