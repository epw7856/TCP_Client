#ifndef COMMUNICATIONSMANAGER_H
#define COMMUNICATIONSMANAGER_H

#include "ConnectionStatus.h"
#include <memory>
#include <QMessageBox>
#include <QThread>
#include <QTimer>

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
    bool getOutboundDataTransmissionStatus() const;
    void connectToServer();
    void disconnectFromServer();

public slots:
    void setSocketPort(unsigned port);
    void setTransmissionPeriodicity(unsigned interval);
    void setConnectionNotificationEnable(bool enabled);
    void stopStartTransmissionTimer(bool timerEnable);
    void receivedConnectionStatusNotification(bool connectionStatus);
    void showSocketErrorMsgPopup(QString msg);
    void updateInboundDataItems(std::vector<unsigned> rawData);

private slots:
    void sendOutboundDataToServer();

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
    QTimer outboundDataTransmissionTimer;
    ConnectionStatus status = ConnectionStatus::Unconnected;
    QThread commsThread;
    std::unique_ptr<SocketProtocol> socketComms;

    void showMessageBox(const QString& title,
                        const QString& msg,
                        QMessageBox::Icon icon);
};

inline ConnectionStatus CommunicationsManager::getConnectionStatus() const
{
    return status;
}

inline void CommunicationsManager::setConnectionNotificationEnable(bool enabled)
{
    showConnectionNotifications = enabled;
}

#endif // COMMUNICATIONSMANAGER_H
