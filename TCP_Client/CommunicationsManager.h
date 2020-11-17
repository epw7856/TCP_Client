#ifndef COMMUNICATIONSMANAGER_H
#define COMMUNICATIONSMANAGER_H

#include <memory>
#include <QThread>

class SocketProtocol;

class CommunicationsManager : public QObject
{
    Q_OBJECT

public:
    explicit CommunicationsManager(unsigned port = 0U);
    ~CommunicationsManager();
    bool isConnectedToServer() const;
    void setSocketPort(unsigned port);
    void connectToServer();
    void disconnectFromServer();

public slots:
    void receivedConnectedNotification();
    void receivedDisconnectedNotification();

signals:
    void requestSetSocketPort(unsigned port);
    void requestConnectToServer();
    void requestDisconnectFromServer();

private:
    bool isConnected = false;
    QThread commsThread;
    std::unique_ptr<SocketProtocol> socketComms;
};

inline bool CommunicationsManager::isConnectedToServer() const
{
    return isConnected;
}

#endif // COMMUNICATIONSMANAGER_H
