#include "CommunicationsManager.h"
#include "QTcpSocket"

CommunicationsManager::CommunicationsManager(unsigned port)
:
    socketPort(port)
{

}

CommunicationsManager::~CommunicationsManager() = default;

void CommunicationsManager::setSocketPort(unsigned port)
{

}

void CommunicationsManager::requestConnectToServer()
{

}

void CommunicationsManager::sendOutboundDataToServer(const std::vector<unsigned> data)
{

}

void CommunicationsManager::requestDisconnectFromServer()
{

}

void CommunicationsManager::connectedToServer()
{

}

void CommunicationsManager::processIncomingTransmission()
{

}

void CommunicationsManager::disconnectedFromServer()
{

}

QByteArray CommunicationsManager::serializeData(std::vector<unsigned>& data)
{

}

std::vector<unsigned> CommunicationsManager::deserializeData(QByteArray& data)
{

}
