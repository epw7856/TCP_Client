#include "CommunicationsManager.h"
#include "InboundDataInterface.h"
#include "OutboundDataInterface.h"
#include <QMessageBox>
#include "QTcpSocket"
#include "SocketProtocol.h"

Q_DECLARE_METATYPE(QString);
Q_DECLARE_METATYPE(std::vector<unsigned>);
Q_DECLARE_METATYPE(unsigned);
Q_DECLARE_METATYPE(bool);

CommunicationsManager::CommunicationsManager(InboundDataInterface& localInboundDataInterface,
                                             OutboundDataInterface& localOutboundDataInterface)
:
    inboundDataInterface(localInboundDataInterface),
    outboundDataInterface(localOutboundDataInterface),
    socketComms(std::make_unique<SocketProtocol>())
{
    qRegisterMetaType<QString>();
    qRegisterMetaType<std::vector<unsigned>>();
    qRegisterMetaType<unsigned>();
    qRegisterMetaType<bool>();

    // Connections from CommunicationsManager to SocketProtocol
    connect(this, &CommunicationsManager::requestDisconnectFromServer, socketComms.get(), &SocketProtocol::disconnectFromServer);
    connect(this, &CommunicationsManager::sendData, socketComms.get(), &SocketProtocol::sendDataToServer);
    connect(this, &CommunicationsManager::requestConnectToServer, socketComms.get(), &SocketProtocol::requestConnectToServer);

    // Connections from SocketProtocol to CommunicationsManager
    connect(socketComms.get(), &SocketProtocol::notifyConnectionStatusUpdate, this, &CommunicationsManager::receivedConnectionStatusNotification);
    connect(socketComms.get(), &SocketProtocol::finishedProcessingInboundData, this, &CommunicationsManager::updateInboundDataItems);
    connect(socketComms.get(), &SocketProtocol::sendErrorMsg, this, &CommunicationsManager::showSocketErrorMsgPopup);

    // Connection for the outbound data transmission timer
    connect(&outboundDataTransmissionTimer, &QTimer::timeout, this, &CommunicationsManager::sendOutboundDataToServer);

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

void CommunicationsManager::setTransmissionPeriodicity(unsigned interval)
{
    bool stopped = false;
    if(outboundDataTransmissionTimer.isActive())
    {
        stopped = true;
        outboundDataTransmissionTimer.stop();
    }

    outboundDataTransmissionTimer.setInterval(interval);

    if(stopped && (status == ConnectionStatus::Connected))
    {
        outboundDataTransmissionTimer.start();
    }
}

void CommunicationsManager::stopStartTransmissionTimer(bool timerEnable)
{
    (timerEnable) ? outboundDataTransmissionTimer.start() : outboundDataTransmissionTimer.stop();
}

void CommunicationsManager::connectToServer()
{
    status = ConnectionStatus::InProgress;

    emit requestConnectToServer(socketPort);
    emit sendStatusUpdate("Connecting to server...");
}

void CommunicationsManager::sendOutboundDataToServer()
{
    emit sendData(outboundDataInterface.getOutboundRawValues());
}

void CommunicationsManager::disconnectFromServer()
{
    status = ConnectionStatus::InProgress;

    emit requestDisconnectFromServer();
    emit sendStatusUpdate("Disconnecting from server...");
}

void CommunicationsManager::receivedConnectionStatusNotification(bool connectionStatus)
{
    if(connectionStatus)
    {
        status = ConnectionStatus::Connected;
        outboundDataTransmissionTimer.start();
    }
    else
    {
        status = ConnectionStatus::Unconnected;
        outboundDataTransmissionTimer.stop();
    }

    QString msg = QString();
    (status == ConnectionStatus::Connected) ? msg = "Connected to server on Port " + QString::number(socketPort) + "." :
                                              msg = "Disconnected from server.";

    if(showConnectionNotifications)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Connection Status");
        msgBox.setText("<p align='center'>" + msg + "</p>");
        msgBox.setFont(QFont("Segoe UI", 10));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }

    emit sendStatusUpdate(msg);
}

void CommunicationsManager::showSocketErrorMsgPopup(QString msg)
{
    status = ConnectionStatus::Unconnected;

    QMessageBox msgBox;
    msgBox.setWindowTitle("Connection Error");
    msgBox.setText("<p align='center'>" + msg + "</p>");
    msgBox.setFont(QFont("Segoe UI", 10));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();

    emit sendStatusUpdate("Connection timeout.");
}

void CommunicationsManager::updateInboundDataItems(std::vector<unsigned> rawData)
{
    inboundDataInterface.setInboundRawValues(rawData);
    emit inboundDataUpdated();
}
