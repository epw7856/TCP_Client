#include "CommunicationsUtilities.h"
#include <QAbstractSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QTcpSocket>
#include "SocketProtocol.h"

// Default connection timeout period of 3 seconds
static const unsigned ConnectionTimeout = 3000;

SocketProtocol::SocketProtocol() {}

SocketProtocol::~SocketProtocol() = default;

void SocketProtocol::requestConnectToServer(unsigned port)
{
    if(socket == nullptr)
    {
        socket = std::make_unique<QTcpSocket>();

        // Connections from internal TCP socket signals to SocketProtocol private slots
        connect(socket.get(), &QTcpSocket::connected, this, &SocketProtocol::connectedToServer);
        connect(socket.get(), &QTcpSocket::disconnected, this, &SocketProtocol::disconnectedFromServer);
        connect(socket.get(), &QTcpSocket::readyRead, this, &SocketProtocol::processIncomingTransmission);
    }

    if(socket->state() != QAbstractSocket::ConnectedState)
    {
        socket->connectToHost(QHostAddress::LocalHost, port);
        if(!socket->waitForConnected(ConnectionTimeout))
        {
            const QString msg = "Connection timeout: Unable to connect to server. Please verify that the socket port is set correctly "
                                "and the server is listening on the same port.";
            emit sendErrorMsg(msg);
        }
    }
}

void SocketProtocol::sendDataToServer(std::vector<unsigned> data)
{
    if((socket->state() == QAbstractSocket::ConnectedState) &&
       (data.size() > 0U))
    {
        socket->write(serializeData(data));
        socket->flush();
    }
}

void SocketProtocol::disconnectFromServer()
{
    if(socket->isOpen())
    {
        socket->close();
    }
}

void SocketProtocol::connectedToServer()
{
    emit notifyConnectionStatusUpdate(true);
}

void SocketProtocol::processIncomingTransmission()
{
    std::vector<unsigned> data = deserializeData(socket->readAll());
    if(data.size() > 0U)
    {
        emit finishedProcessingInboundData(data);
    }
}

void SocketProtocol::disconnectedFromServer()
{
    emit notifyConnectionStatusUpdate(false);
}

QByteArray SocketProtocol::serializeData(std::vector<unsigned> data)
{
    return toByteArray(data);
}

std::vector<unsigned> SocketProtocol::deserializeData(QByteArray data)
{
    return toUint32Vector(data);
}
