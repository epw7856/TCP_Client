#ifndef COMMUNICATIONSMANAGER_H
#define COMMUNICATIONSMANAGER_H

#include "ConnectionStatus.h"
#include <memory>
#include <QThread>

class InboundDataInterface;
class OutboundDataInterface;
class SocketProtocol;

class CommunicationsManager : public QObject
{
    Q_OBJECT

public:
    explicit CommunicationsManager(InboundDataInterface& localInboundDataInterface,
                                   OutboundDataInterface& localOutboundDataInterface);
    ~CommunicationsManager();

    ConnectionStatus getConnectionStatus() const;
    void setConnectionNoticationEnable(bool enabled);
    void setSocketPort(unsigned port);
    void connectToServer();
    void sendOutboundDataToServer();
    void disconnectFromServer();

public slots:
    void receivedConnectionStatusNotification(bool connectionStatus);
    void showSocketErrorMsgPopup(QString msg);
    void updateInboundDataItems(std::vector<unsigned> rawData);

signals:
    void requestConnectToServer(unsigned port);
    void sendData(std::vector<unsigned> data);
    void requestDisconnectFromServer();
    void sendStatusUpdate(QString msg);
    void inboundDataUpdated();

private:
    InboundDataInterface& inboundDataInterface;
    OutboundDataInterface& outboundDataInterface;
    bool showConnectionNotifications = false;
    unsigned socketPort = 0U;
    ConnectionStatus status = ConnectionStatus::Unconnected;
    QThread commsThread;
    std::unique_ptr<SocketProtocol> socketComms;
};

inline ConnectionStatus CommunicationsManager::getConnectionStatus() const
{
    return status;
}

inline void CommunicationsManager::setConnectionNoticationEnable(bool enabled)
{
    showConnectionNotifications = enabled;
}

#endif // COMMUNICATIONSMANAGER_H
