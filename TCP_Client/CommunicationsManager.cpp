#include "CommunicationsManager.h"
#include "QTcpSocket"
#include "SocketProtocol.h"

CommunicationsManager::CommunicationsManager(unsigned port)
:
    socketComms(std::make_unique<SocketProtocol>(port))
{
    // Connections from CommunicationsManager to SocketProtocol
    connect(this, &CommunicationsManager::requestDisconnectFromServer, socketComms.get(), &SocketProtocol::requestDisconnectFromServer);
    connect(this, &CommunicationsManager::requestConnectToServer, socketComms.get(), &SocketProtocol::requestConnectToServer);

    // Connections from SocketProtocol to CommunicationsManager
    connect(socketComms.get(), &SocketProtocol::notifyConnectedToServer, this, &CommunicationsManager::receivedConnectedNotification);
    connect(socketComms.get(), &SocketProtocol::notifyDisconnectedFromServer, this, &CommunicationsManager::receivedDisconnectedNotification);

    socketComms->moveToThread(&commsThread);
    commsThread.start();
}

CommunicationsManager::~CommunicationsManager()
{
    emit requestDisconnectFromServer();
    commsThread.quit();
    commsThread.wait(2000);
}

void CommunicationsManager::setSocketPort(unsigned port)
{
    emit requestSetSocketPort(port);
}

void CommunicationsManager::connectToServer()
{
    emit requestConnectToServer();
}

void CommunicationsManager::disconnectFromServer()
{
    emit requestConnectToServer();
}

void CommunicationsManager::receivedConnectedNotification()
{
    isConnected = true;
}

void CommunicationsManager::receivedDisconnectedNotification()
{
    isConnected = false;
}
