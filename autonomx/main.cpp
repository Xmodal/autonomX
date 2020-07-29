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
#include "SpikingNet.h"
#include "AppModel.h"

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

    AppModel::getInstance().createGenerator();
    AppModel::getInstance().start();

    QQmlApplicationEngine qmlEngine;

    // Pass C++ objects to QML.
    qmlEngine.rootContext()->setContextProperty("generatorModel", AppModel::getInstance().getGeneratorModel());
    qmlEngine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (qmlEngine.rootObjects().isEmpty())
        return -1;

    // end compute and osc threads on application exit
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [](){
        AppModel::getInstance().getComputeThread()->exit();
        AppModel::getInstance().getOscThread()->exit();
    });

    // disable App Nap on macOS to prevent compute / osc thread from stalling
    #ifdef Q_OS_MAC
    disableAppNap();
    #endif

    // start QML application and return error code
    return app.exec();
}
