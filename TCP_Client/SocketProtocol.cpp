#include "QTcpSocket"
#include "SocketProtocol.h"

SocketProtocol::SocketProtocol(unsigned port)
:
    socketPort(port)
{

}

SocketProtocol::~SocketProtocol() = default;

void SocketProtocol::setSocketPort(unsigned port)
{

}

void SocketProtocol::requestConnectToServer()
{

}

void SocketProtocol::sendOutboundDataToServer(const std::vector<unsigned> data)
{

}

void SocketProtocol::requestDisconnectFromServer()
{

}

void SocketProtocol::connectedToServer()
{

}

void SocketProtocol::processIncomingTransmission()
{

}

void SocketProtocol::disconnectedFromServer()
{

}

QByteArray SocketProtocol::serializeData(std::vector<unsigned>& data)
{

}

std::vector<unsigned> SocketProtocol::deserializeData(QByteArray& data)
{

}
