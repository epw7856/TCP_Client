#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <memory>
#include <QThread>
#include <QObject>

class CommunicationsManager;
class ConfigFileVerificationHandler;
class SystemDataSource;

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController(const QString& configFilePath);
    ~MainWindowController();

public slots:
    void receivedConnectedNotification();
    void receivedDisconnectedNotification();

signals:
    void requestConnectToServer();
    void requestDisconnectFromServer();

private:
    QThread commsThread;
    std::unique_ptr<SystemDataSource> sds;
    std::unique_ptr<ConfigFileVerificationHandler> verifier;
    std::unique_ptr<CommunicationsManager> commsManager;
    bool isConnectedToServer = false;

    void loadConfiguration(const QString& configFilePath);
};

#endif // MAINWINDOWCONTROLLER_H
