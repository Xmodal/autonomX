#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "oscreceiver.h"
#include "oscsender.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    int argumentCount = QCoreApplication::arguments().size();
    QStringList argumentList = QCoreApplication::arguments();
    QTextStream standardOutput(stdout);

    QString sendOscHost = "127.0.0.1";
    quint16 sendOscPort = 14444;
    quint16 receiveOscPort = 14444;

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

    standardOutput << QObject::tr("Receive OSC on port %1").arg(receiveOscPort) << endl;
    standardOutput << QObject::tr("Send OSC to %1:%2").arg(sendOscHost).arg(sendOscPort) << endl;

    QQmlApplicationEngine engine;

    // add parent directory as import path
    // CUSTOM_IMPORT variable is set in DynamicLights.pro
    QDir dir(QString(CUSTOM_IMPORT));
    dir.cd("../");
    engine.addImportPath(dir.path());

    OscReceiver oscReceiver(receiveOscPort);
    OscSender oscSender(sendOscHost, sendOscPort);

    // Pass C++ objects to QML.
    engine.rootContext()->setContextProperty("oscSender", &oscSender);
    engine.rootContext()->setContextProperty("oscReceiver", &oscReceiver);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
