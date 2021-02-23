#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include "InboundDataTableModel.h"
#include <memory>
#include "OutboundDataTableModel.h"

class AboutDialog;
class ApplicationSettingsDialog;
class CommunicationsManager;
class InboundDataInterface;
class OutboundDataInterface;
class RangeCheckHandler;
class SettingsManager;
class SystemDataSource;

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController(const QString& configFilePath);
    ~MainWindowController();

    QString getHeaderFooterText() const;

    InboundDataTableModel& getInboundDataTableModel();
    OutboundDataTableModel& getOutboundDataTableModel();

    void performInitialSetup();
    void requestConnectToServer();
    void requestDisconnectFromServer();

    void selectConfigurationFile(QWidget* parent);

    bool enableFileActionsButton() const;
    bool enableActionConnectToServer() const;
    bool enableActionDisconnectFromServer() const;
    bool enableClearButton() const;
    bool enableResetButton() const;

    void outboundTableDoubleClicked(const QModelIndex& index);
    void applyDesiredOutboundValues();
    void clearDesiredOutboundValues();
    void resetDesiredOutboundValuesToDefaults();
    void resetSelectedDesiredOutboundValuesToDefaults(const QModelIndexList& selection);

    void showAboutDialog(QWidget* parent);
    void showApplicationSettingsDialog(QWidget* parent);

    QSize getMainWindowSizeSetting() const;
    void saveMainWindowSizeSetting(QSize size);
    bool getMainWindowMaximizedSetting() const;
    void saveMainWindowMaximizedSetting(bool maximized);

    QString getMaxTempIndiv() const;
    QString getMaxTempGroup() const;
    QString getMinTempIndiv() const;
    QString getMinTempGroup() const;
    QString getSetPoint1() const;
    QString getSetPoint2() const;
    QString getCurrentMode() const;
    QString getCommandedMode() const;
    QString getEquipmentStatus() const;
    QString getLinkDisplayStatus() const;
    void setModeCommand(bool mode1Command);
    void setEquipmentStatus(bool openCommand);
    void setLinkStatus(bool link1EnabledCommand);
    bool areFaultsPresent() const;

public slots:
    void updateInboundDataDisplay();
    void receivedStatusUpdate(QString msg);
    void refreshSettings(bool reconnect);

signals:
    void notifyDataUpdated();
    void sendStatusBarMessage(QString msg);
    void notifyStatusChange();
    void requestMainWindowUpdate();

private:
    std::unique_ptr<SystemDataSource> sds;
    std::unique_ptr<CommunicationsManager> commsManager;
    std::unique_ptr<SettingsManager> settingsManager;
    std::unique_ptr<AboutDialog> aboutDialog;
    std::unique_ptr<ApplicationSettingsDialog> appSettingsDialog;
    std::unique_ptr<RangeCheckHandler> rangeCheckHandler;
    bool configurationLoaded = false;
    InboundDataTableModel inboundDataTableModel;
    OutboundDataTableModel outboundDataTableModel;

    void loadConfiguration(const QString& configFilePath, bool initialLoad);
    void executeConnect();
    void executeDisconnect();
    void showUserActionErrorPopup(const QString& title, const QString& msg);
    void validateOutboundCommand(const QString& dataItemName, QString& input);
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
