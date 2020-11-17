#ifndef COMMUNICATIONSMANAGER_H
#define COMMUNICATIONSMANAGER_H

#include <memory>
#include <QByteArray>
#include <QObject>

class QTcpSocket;

class CommunicationsManager : public QObject
{
    Q_OBJECT

public:
    explicit CommunicationsManager(unsigned port = 0U);
    ~CommunicationsManager();

public slots:
    void setSocketPort(unsigned port);
    void requestConnectToServer();
    void sendOutboundDataToServer(const std::vector<unsigned> data);
    void requestDisconnectFromServer();

private slots:
    void connectedToServer();
    void processIncomingTransmission();
    void disconnectedFromServer();

signals:
    void notifyConnectedToServer();
    void notifyDisconnectedFromServer();
    void finishedProcessingInboundData(std::vector<unsigned> data);

private:
    unsigned socketPort = 0U;
    std::unique_ptr<QTcpSocket> socket;

    QByteArray serializeData(std::vector<unsigned>& data);
    std::vector<unsigned> deserializeData(QByteArray& data);
};

#endif // COMMUNICATIONSMANAGER_H
