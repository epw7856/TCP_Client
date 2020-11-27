#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <memory>
#include <QObject>

class CommunicationsManager;
class ConfigFileVerificationHandler;
class InboundDataInterface;
class OutboundDataInterface;
class SystemDataSource;

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController(const QString& configFilePath);
    ~MainWindowController();

    InboundDataInterface& getInboundDataInterface() const;
    OutboundDataInterface& getOutboundDataInterface() const;

    void performInitialSetup();
    void requestConnectToServer();
    void requestDisconnectFromServer();

    bool enableActionConnectToServer() const;
    bool enableActionDisconnectFromServer() const;
    bool enableButtonSaveToFile() const;
    bool enableButtonRestoreFromFile() const;

public slots:
    void updateInboundDataDisplay();
    void receivedStatusUpdate(QString msg);

signals:
    void notifyInboundDataUpdated();
    void sendStatusBarMessage(QString msg);
    void notifyStatusChange();

private:
    std::unique_ptr<SystemDataSource> sds;
    std::unique_ptr<ConfigFileVerificationHandler> verifier;
    std::unique_ptr<CommunicationsManager> commsManager;
    bool configurationLoaded = false;

    void loadConfiguration(const QString& configFilePath);
    void executeConnect();
    void executeDisconnect();
    void showUserActionErrorPopup(const QString& title, const QString& msg);
};

#endif // MAINWINDOWCONTROLLER_H
