//
// Run in release mode for better performances
//

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QObject>
#include "oscsender.h"
#include "oscreceiver.h"

int main(int argc, char *argv[])
{
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
        sendOscPort = argumentList.at(2).toInt();
    }
    if (argumentCount > 3) {
        receiveOscPort = argumentList.at(3).toInt();
    }
    if (argumentCount == 1) {
        standardOutput << QObject::tr("To specify an OSC send host and port: %1 <sendHost> <sendPort> <receivePort").arg(
                argumentList.first()) << endl;
    }

    standardOutput << QObject::tr("Receive OSC on port %1").arg(receiveOscPort) << endl;
    standardOutput << QObject::tr("Send OSC to %1:%2").arg(sendOscHost).arg(sendOscPort) << endl;

    OscReceiver oscReceiver(receiveOscPort);
    OscSender oscSender(sendOscHost, sendOscPort);

    QQmlApplicationEngine engine;
    // Pass C++ objects to QML.
    // Note: It's considered bad practice to do the opposite. Always access C++ methods, signals and properties from QML,
    // and not the other way around.
    engine.rootContext()->setContextProperty("oscSender", &oscSender);
    engine.rootContext()->setContextProperty("oscReceiver", &oscReceiver);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    int ret = app.exec();
    return ret;
}
