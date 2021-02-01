#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include "InboundDataTableModel.h"
#include <memory>
#include "OutboundDataTableModel.h"
#include <QObject>

class CommunicationsManager;
class InboundDataInterface;
class OutboundDataInterface;
class SettingsManager;
class SystemDataSource;

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController(const QString& configFilePath);
    ~MainWindowController();

    InboundDataTableModel& getInboundDataTableModel();
    OutboundDataTableModel& getOutboundDataTableModel();

    void performInitialSetup();
    void requestConnectToServer();
    void requestDisconnectFromServer();

    void selectConfigurationFile(QWidget* parent);

    bool enableFileActionsMenu() const;
    bool enableActionConnectToServer() const;
    bool enableActionDisconnectFromServer() const;
    bool enableClearButton() const;
    bool enableResetButton() const;

    void applyDesiredOutboundValues();
    void clearDesiredOutboundValues();
    void resetDesiredOutboundValuesToDefaults();

public slots:
    void updateInboundDataDisplay();
    void receivedStatusUpdate(QString msg);

signals:
    void notifyInboundDataUpdated();
    void sendStatusBarMessage(QString msg);
    void notifyStatusChange();

private:
    std::unique_ptr<SystemDataSource> sds;
    std::unique_ptr<CommunicationsManager> commsManager;
    std::unique_ptr<SettingsManager> settingsManager;
    bool configurationLoaded = false;
    InboundDataTableModel inboundDataTableModel;
    OutboundDataTableModel outboundDataTableModel;

    void loadConfiguration(const QString& configFilePath, bool initialLoad);
    void executeConnect();
    void executeDisconnect();
    void showUserActionErrorPopup(const QString& title, const QString& msg);
};

inline InboundDataTableModel& MainWindowController::getInboundDataTableModel()
{
    return inboundDataTableModel;
}

inline OutboundDataTableModel& MainWindowController::getOutboundDataTableModel()
{
    return outboundDataTableModel;
}

#endif // MAINWINDOWCONTROLLER_H
