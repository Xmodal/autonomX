#include "OscReceiver.h"
#include "contrib/oscpack/OscTypes.h"
#include "contrib/oscpack/OscReceivedElements.h"
#include <chrono>
#include <QDebug>
#include <QThread>

OscReceiver::OscReceiver(quint16 port, QObject* parent) :
        QObject(parent)
{
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (OscReceiver):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    m_udpSocket = new QUdpSocket(this);
    m_port = port;
    m_udpSocket->bind(QHostAddress::Any, m_port);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &OscReceiver::readyReadCb);
}

OscReceiver::~OscReceiver()
{
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (OscReceiver):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    m_udpSocket->close();
    delete m_udpSocket;
}

void OscReceiver::setPort(quint16 port)
{
    m_port = port;
    m_udpSocket->close();
    m_udpSocket->bind(QHostAddress::Any, m_port);
}

void OscReceiver::readyReadCb() {
    while (this->m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = this->m_udpSocket->receiveDatagram();
        // XXX: we could also retrieve the sender host and port
        QByteArray data = datagram.data();
        QVariantList arguments;
        QString oscAddress;
        controlMessageBool = false;

//        qDebug() << "oscReceiver class data message: " << data;
        if(data.contains("_")) {
            controlMessageBool = true;
//            qDebug() << "01 received control message - OscReceiver.cpp";
//            this->byteArrayToVariantList(arguments, oscAddress, data);
////            emit controlMessageReceived(oscAddress, arguments);
//            return;
        }

        this->byteArrayToVariantList(arguments, oscAddress, data);
        emit messageReceived(oscAddress, arguments, controlMessageBool);
    }
}

void OscReceiver::byteArrayToVariantList(QVariantList& outputVariantList, QString& outputOscAddress, const QByteArray& inputByteArray) {
    osc::ReceivedPacket packet(inputByteArray.data(), inputByteArray.size());
    // TODO: catch parsing exceptions
    if (packet.IsMessage()) {
        osc::ReceivedMessage message(packet);
        // Get address pattern
        QString address(message.AddressPattern());
        outputOscAddress.replace(0, address.size(), address);

        for (auto iter = message.ArgumentsBegin(); iter != message.ArgumentsEnd(); ++ iter) {
            osc::ReceivedMessageArgument argument = (*iter);
            if (argument.IsBool()) {
                outputVariantList.append(QVariant(argument.AsBool()));
            } else if (argument.IsString()) {
                outputVariantList.append(QVariant(argument.AsString()));
            } else if (argument.IsInt32()) {
#ifdef Q_OS_WIN32
                // For some windows system this can prevent from compilation error
                outputVariantList.append(QVariant(qulonglong(argument.AsInt32())));
#else
              outputVariantList.append(QVariant(argument.AsInt32()));
#endif
            } else if (argument.IsFloat()) {
                outputVariantList.append(QVariant(argument.AsFloat()));
            } else if (argument.IsChar()) {
                outputVariantList.append(QVariant(argument.AsChar()));
            //} else if (argument.IsInt64()) {
            //    outputVariantList.append(QVariant(argument.AsInt64()));
            } else if (argument.IsDouble()) {
                outputVariantList.append(QVariant(argument.AsDouble()));
            }
            // TODO: support Array, Midi, Blob, Symbol, TimeTag, RGBA, Nil
        }
    } // TODO: also parse bundles
}
