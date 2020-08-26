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
#include "GeneratorRegion.h"
#include "GeneratorRegionModel.h"
#include "GeneratorLattice.h"
#include "SpikingNet.h"
#include "AppModel.h"

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


    // expose custom types to QML and Qt's meta type system
    qmlRegisterUncreatableType<Generator>("ca.hexagram.xmodal.autonomx", 1, 0, "Generator", "Cannot instanciate Generator.");
    qmlRegisterUncreatableType<GeneratorFacade>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorFacade", "Cannot instanciate GeneratorFacade.");
    qmlRegisterUncreatableType<GeneratorRegion>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorRegion", "Cannot instanciate GeneratorRegion.");
    qmlRegisterUncreatableType<GeneratorRegionModel>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorRegionModel", "Cannot instanciate GeneratorRegionModel.");
    qmlRegisterUncreatableType<SpikingNet>("ca.hexagram.xmodal.autonomx", 1, 0, "SpikingNet", "Cannot instanciate SpikingNet.");
    qmlRegisterUncreatableType<NeuronType>("ca.hexagram.xmodal.autonomx", 1, 0, "NeuronType", "Cannot instanciate NeuronType.");
    qmlRegisterType<GeneratorLattice>("ca.hexagram.xmodal.autonomx", 1, 0, "GeneratorLattice");
    qRegisterMetaType<QSharedPointer<Generator>>();

    AppModel::getInstance().createGenerator();

    QByteArray intensityValueFromString;
    intensityValueFromString.append(QString("intensity"));
    QByteArray intensityValueFromRole = GeneratorRegionModel::roleMap.value(GeneratorRegionModel::GeneratorRegionRoles::IntensityRole);
    qDebug() << "intensity QByteArray from string:  " << intensityValueFromString;
    qDebug() << "intensity QByteArray from role: " << intensityValueFromRole;

    int intensityKeyFromString = GeneratorRegionModel::roleMap.key(intensityValueFromString);
    int intensityKeyFromRole = GeneratorRegionModel::roleMap.key(intensityValueFromRole);
    qDebug() << "intensity int from string: " << intensityKeyFromString;
    qDebug() << "intensity int from role:   " << intensityKeyFromRole;

    QQmlApplicationEngine qmlEngine;

    // Pass C++ objects to QML.
    qmlEngine.rootContext()->setContextProperty("appModel", &AppModel::getInstance());
    qmlEngine.rootContext()->setContextProperty("generatorModel", AppModel::getInstance().getGeneratorModel());
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
