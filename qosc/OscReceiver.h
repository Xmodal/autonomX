#pragma once

#include <QObject>
#include <QVariant>
#include <QtNetwork>
#include <QHostAddress>

/**
 * @brief Receives OSC on a given port number.
 *
 * Currently only supports unicast UDP.
 */
class OscReceiver : public QObject
{
    Q_OBJECT

    // TODO: Support multicast UDP
    // TODO: Support TCP
    // TODO: Support broadcast UDP
    // TODO: Support OSC bundles
    // TODO: Provide a class to route messages.
public:
    /**
     * @brief Constructor.
     * @param receivePort Port number to listen to.
     */
    explicit OscReceiver(quint16 port, QObject *parent = nullptr);
    ~OscReceiver();
    void setPort(quint16 port);
    bool controlMessageBool;

signals:
    /**
     * @brief Signal triggered each time we receive a message.
     * @param oscAddress
     * @param message
     */
    void messageReceived(const QString& oscAddress, const QVariantList& message, bool controlMessage);

public slots:
    void readyReadCb();

private:
    QUdpSocket* m_udpSocket;
    quint16 m_port;

    void byteArrayToVariantList(QVariantList& outputVariantList, QString& outputOscAddress, const QByteArray& inputByteArray);

    bool flagDebug = false;
};
