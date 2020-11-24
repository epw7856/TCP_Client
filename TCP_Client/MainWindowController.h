#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <memory>
#include <QObject>

class CommunicationsManager;
class ConfigFileVerificationHandler;
class InboundDataInterface;
class SystemDataSource;

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController(const QString& configFilePath);
    ~MainWindowController();
    InboundDataInterface& getInboundDataInterface() const;
    void requestConnectToServer();

public slots:
    void updateInboundDataDisplay();
    void receivedStatusUpdate(QString msg);

signals:
    void notifyInboundDataUpdated();
    void sendStatusBarMessage(QString msg);

private:
    std::unique_ptr<SystemDataSource> sds;
    std::unique_ptr<ConfigFileVerificationHandler> verifier;
    std::unique_ptr<CommunicationsManager> commsManager;
    bool configurationLoaded = false;

    void loadConfiguration(const QString& configFilePath);
};

#endif // MAINWINDOWCONTROLLER_H
