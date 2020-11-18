#include "CommunicationsManager.h"
#include "InboundDataInterface.h"
#include "OutboundDataInterface.h"
#include <QMessageBox>
#include "QTcpSocket"
#include "SocketProtocol.h"

CommunicationsManager::CommunicationsManager(InboundDataInterface& localInboundDataInterface,
                                             OutboundDataInterface& localOutboundDataInterface,
                                             unsigned port)
:
    inboundDataInterface(localInboundDataInterface),
    outboundDataInterface(localOutboundDataInterface),
    socketPort(port),
    socketComms(std::make_unique<SocketProtocol>())
{
    // Connections from CommunicationsManager to SocketProtocol
    connect(this, &CommunicationsManager::requestDisconnectFromServer, socketComms.get(), &SocketProtocol::disconnectFromServer);
    connect(this, &CommunicationsManager::sendData, socketComms.get(), &SocketProtocol::sendDataToServer);
    connect(this, &CommunicationsManager::requestConnectToServer, socketComms.get(), &SocketProtocol::requestConnectToServer);

    // Connections from SocketProtocol to CommunicationsManager
    connect(socketComms.get(), &SocketProtocol::notifyConnectionStatusUpdate, this, &CommunicationsManager::receivedConnectionStatusNotification);
    connect(socketComms.get(), &SocketProtocol::finishedProcessingInboundData, this, &CommunicationsManager::updateInboundDataItems);

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
    socketPort = port;
}

void CommunicationsManager::connectToServer()
{
    emit requestConnectToServer(socketPort);
}

void CommunicationsManager::sendOutboundDataToServer()
{
    emit sendData(outboundDataInterface.getOutboundRawValues());
}

void CommunicationsManager::disconnectFromServer()
{
    emit requestDisconnectFromServer();
}

void CommunicationsManager::receivedConnectionStatusNotification(bool connectionStatus)
{
    isConnected = connectionStatus;
}

void CommunicationsManager::receivedErrorMsgFromSocket(QString msg)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Connection Error");
    msgBox.setText("<p align='center'>" + msg + "</p>");
    msgBox.setFont(QFont("Segoe UI", 10));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void CommunicationsManager::updateInboundDataItems(std::vector<unsigned> rawData)
{
    inboundDataInterface.setInboundRawValues(rawData);
}
