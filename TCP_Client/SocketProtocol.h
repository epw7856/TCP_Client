#ifndef SOCKETPROTOCOL_H
#define SOCKETPROTOCOL_H

#include <memory>
#include <QByteArray>
#include <QObject>

class QTcpSocket;

class SocketProtocol : public QObject
{
    Q_OBJECT

public:
    SocketProtocol();
    ~SocketProtocol();

public slots:
    void requestConnectToServer(unsigned port);
    void sendDataToServer(const std::vector<unsigned> data);
    void disconnectFromServer();

private slots:
    void connectedToServer();
    void processIncomingTransmission();
    void disconnectedFromServer();

signals:
    void notifyConnectionStatusUpdate(bool connectionStatus);
    void finishedProcessingInboundData(std::vector<unsigned> data);
    void sendErrorMsg(QString msg);

private:
    unsigned socketPort = 0U;
    std::unique_ptr<QTcpSocket> socket;

    QByteArray serializeData(std::vector<unsigned> data);
    std::vector<unsigned> deserializeData(QByteArray data);
};

#endif // SOCKETPROTOCOL_H
