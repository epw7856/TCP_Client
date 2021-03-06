#include "AboutDialog.h"
#include "ApplicationSettingsDialog.h"
#include "CommunicationsManager.h"
#include "DataItem.h"
#include "EnumDefinitionNames.h"
#include "EnumSelectionDialog.h"
#include "FileOperationsHandler.h"
#include "InboundDataItemNames.h"
#include "MainWindowController.h"
#include "OutboundDataItemNames.h"
#include <QMessageBox>
#include "RangeCheckHandler.h"
#include "SettingsManager.h"
#include "SystemDataSource.h"

MainWindowController::MainWindowController(const QString& configFilePath)
:
    sds(std::make_unique<SystemDataSource>()),
    commsManager(std::make_unique<CommunicationsManager>(*sds, *sds)),
    settingsManager(std::make_unique<SettingsManager>()),
    rangeCheckHandler(std::make_unique<RangeCheckHandler>(*sds, *sds)),
    fileOperationsHandler(std::make_unique<FileOperationsHandler>(*sds, *settingsManager)),
    inboundDataTableModel(*sds),
    outboundDataTableModel(*sds, *sds)
{   
    if(!configFilePath.isEmpty())
    {
        loadConfiguration(configFilePath, true);
    }

    // Connections from CommunicationsManager to MainWindowController
    connect(commsManager.get(), &CommunicationsManager::sendStatusUpdate, this, &MainWindowController::receivedStatusUpdate);
    connect(commsManager.get(), &CommunicationsManager::inboundDataUpdated, this, &MainWindowController::updateInboundDataDisplay);

    connect(fileOperationsHandler.get(), &FileOperationsHandler::transmitRestoredDisplayValues, this, &MainWindowController::receivedRestoredControlData);
}

QString MainWindowController::getHeaderFooterText() const
{
    return sds->getHeaderFooterText();
}

MainWindowController::~MainWindowController() = default;

void MainWindowController::requestConnectToServer()
{
    if(!configurationLoaded)
    {
        showUserActionErrorPopup("Configuration Error",
                                 "Please load a configuration file prior to connecting to the server.");
    }
    else if(sds->getSocketPort() <= 0U)
    {
        showUserActionErrorPopup("Socket Port Error",
                                 "Please configure the local socket port number prior to connecting to the server.");
    }
    else if(sds->getTransmissionPeriodicity() <= 0U)
    {
        showUserActionErrorPopup("Transmission Periodicity Error",
                                 "Please configure the transmission periodicity setting prior to connecting to the server.");
    }
    else
    {
        executeConnect();
    }
}

void MainWindowController::requestDisconnectFromServer()
{
    if(commsManager->getConnectionStatus() != ConnectionStatus::Connected)
    {
        showUserActionErrorPopup("Connection Error", "There is currently no active connection with the server.");
    }
    else
    {
        executeDisconnect();
    }
}

void MainWindowController::selectConfigurationFile(QWidget* parent)
{
    bool status = true;
    if((commsManager->getConnectionStatus() == ConnectionStatus::Connected) &&
       (commsManager->getOutboundDataTransmissionStatus()))
    {
        status = FileOperationsHandler::showConfigFileSelectionWarning();
    }

    if(status)
    {
        const QString configFile = fileOperationsHandler->showFileSelectionDialog("Select Configuration File", parent);
        loadConfiguration(configFile, false);
    }
}

bool MainWindowController::enableFileActionsButton() const
{
    return configurationLoaded;
}

bool MainWindowController::enableActionConnectToServer() const
{
    return (configurationLoaded && (commsManager->getConnectionStatus() == ConnectionStatus::Unconnected));
}

bool MainWindowController::enableActionDisconnectFromServer() const
{
    return (commsManager->getConnectionStatus() == ConnectionStatus::Connected);
}

bool MainWindowController::enableApplyButton() const
{
    return configurationLoaded;
}

bool MainWindowController::enableClearButton() const
{
    return configurationLoaded;
}

bool MainWindowController::enableRestoreButton() const
{
    return configurationLoaded;
}

void MainWindowController::outboundTableDoubleClicked(const QModelIndex& index)
{
    if(index.column() == outboundDataTableModel.getNewValueColumn())
    {
        DataItem* item = sds->getOutboundDataItem(index.sibling(index.row(), 0).data().toInt());
        if(item->isEnumType())
        {
            EnumSelectionDialog enumDialog;
            enumDialog.setDataItemName(item->getDataItemName());
            enumDialog.setEnumName(item->getDataItemType());
            enumDialog.setEnumStrings(sds->getEnumStrings(item->getDataItemType()));
            enumDialog.setCurrentValue(item->getDisplayValue());
            enumDialog.exec();

            QString newVal;
            (enumDialog.isAccepted()) ? newVal = enumDialog.getNewValue() :
                                        newVal = QString();

            outboundDataTableModel.setData(index, newVal, Qt::EditRole);
        }
    }
}

void MainWindowController::applyDesiredOutboundValues()
{
    outboundDataTableModel.applyDesiredOutboundValues();
}

void MainWindowController::clearAllDesiredOutboundValues()
{
    outboundDataTableModel.clearDesiredOutboundValues();
}

void MainWindowController::restoreAllDesiredOutboundValuesToDefaults()
{
    outboundDataTableModel.resetDesiredOutboundValuesToDefaults();
}

void MainWindowController::restoreSelectedDesiredOutboundValuesToDefaults(const QModelIndexList& selection)
{
    std::vector<unsigned> indices;
    for(const auto& i : selection)
    {
        indices.push_back(i.row());
    }

    outboundDataTableModel.resetDesiredOutboundValuesToDefaults(indices);
}

void MainWindowController::clearSelectedDesiredOutboundValues(const QModelIndexList& selection)
{
    std::vector<unsigned> indices;
    for(const auto& i : selection)
    {
        indices.push_back(i.row());
    }

    outboundDataTableModel.clearDesiredOutboundValues(indices);
}

void MainWindowController::saveControlDataToFile(QWidget* parent)
{
    fileOperationsHandler->initiateSaveControlDataToFile(outboundDataTableModel.getOutboundDataItemMap(), parent);
}

void MainWindowController::restoreControlDataFromFile(QWidget* parent)
{
    fileOperationsHandler->initiateRestoreControlDataFromFile(outboundDataTableModel.getOutboundDataItemMap(), parent);
}

void MainWindowController::showAboutDialog(QWidget* parent)
{
    if(aboutDialog == nullptr)
    {
        aboutDialog = std::make_unique<AboutDialog>(*sds, parent);
    }

    aboutDialog->show();
    aboutDialog->raise();
    aboutDialog->activateWindow();
}

void MainWindowController::showApplicationSettingsDialog(QWidget* parent)
{
    if(appSettingsDialog == nullptr)
    {
        appSettingsDialog = std::make_unique<ApplicationSettingsDialog>(*sds, *settingsManager, parent);

        // Connection from the Application Settings Dialog to the MainWindowController
        connect(appSettingsDialog.get(), &ApplicationSettingsDialog::requestSettingsRefresh, this, &MainWindowController::refreshSettings);
    }

    appSettingsDialog->show();
    appSettingsDialog->raise();
    appSettingsDialog->activateWindow();
}

QSize MainWindowController::getMainWindowSizeSetting() const
{
    return settingsManager->getMainWindowSizeSetting();
}

void MainWindowController::saveMainWindowSizeSetting(QSize size)
{
    settingsManager->setMainWindowSizeSetting(size);
}

bool MainWindowController::getMainWindowMaximizedSetting() const
{
    return settingsManager->getMainWindowMaximizedSetting();
}

void MainWindowController::saveMainWindowMaximizedSetting(bool maximized)
{
    settingsManager->setMainWindowMaximizedSetting(maximized);
}

QString MainWindowController::getMaxTempIndiv() const
{
    return sds->getInboundDataItemDisplayValue(MaxTempIndiv);
}

QString MainWindowController::getMaxTempGroup() const
{
    return sds->getInboundDataItemDisplayValue(MaxTempGroup);
}

QString MainWindowController::getMinTempIndiv() const
{
    return sds->getInboundDataItemDisplayValue(MinTempIndiv);
}

QString MainWindowController::getMinTempGroup() const
{
    return sds->getInboundDataItemDisplayValue(MinTempGroup);
}

QString MainWindowController::getSetPoint1() const
{
    return sds->getInboundDataItemDisplayValue(SetPoint1);
}

QString MainWindowController::getSetPoint2() const
{
    return sds->getInboundDataItemDisplayValue(SetPoint2);
}

QString MainWindowController::getCurrentMode() const
{
    return sds->getInboundDataItemDisplayValue(CurrentOpMode);
}

QString MainWindowController::getCommandedMode() const
{
    return sds->getOutboundDataItemDisplayValue(CurrentOpModeCommand);
}

QString MainWindowController::getEquipmentStatus() const
{
    return sds->getInboundDataItemDisplayValue(EquipmentStatus);
}

QString MainWindowController::getLinkDisplayStatus() const
{
    return sds->getInboundDataItemDisplayValue(LinkDisplayStatus);
}

void MainWindowController::setModeCommand(bool mode1Command)
{
    QString modeOption;
    (mode1Command) ? modeOption = enumOpModeMode1 :
                     modeOption = enumOpModeMode2;

    validateOutboundCommand(CurrentOpModeCommand, modeOption);
}

void MainWindowController::setEquipmentStatus(bool openCommand)
{
    QString statusOption;
    (openCommand) ? statusOption = enumStatusOpened :
                    statusOption = enumStatusClosed;

    validateOutboundCommand(EquipmentStatusCommand, statusOption);
}

void MainWindowController::setLinkStatus(bool link1EnabledCommand)
{
    QString enableOption;
    (link1EnabledCommand) ? enableOption = enumLinkStatusLink1Enabled :
                            enableOption = enumLinkStatusLink2Enabled;

    validateOutboundCommand(LinkDisplayStatusCommand, enableOption);
}

bool MainWindowController::areFaultsPresent() const
{
    return (sds->getInboundDataItemDisplayValue(FaultStatus) == enumBooleanTrue);
}

void MainWindowController::updateInboundDataDisplay()
{
    emit inboundDataTableModel.layoutChanged();
    emit notifyDataUpdated();
}

void MainWindowController::receivedStatusUpdate(QString msg)
{
    emit sendStatusBarMessage(msg);
    emit notifyStatusChange();
}

void MainWindowController::refreshSettings(bool reconnect)
{
    commsManager->setConnectionNotificationEnable(settingsManager->getShowConnectionNotificationsSetting());
    commsManager->setSocketPort(sds->getSocketPort());
    commsManager->setTransmissionPeriodicity(sds->getTransmissionPeriodicity());

    if(reconnect && (commsManager->getConnectionStatus() == ConnectionStatus::Connected))
    {
       executeDisconnect();
       executeConnect();
    }
}

void MainWindowController::receivedRestoredControlData(std::vector<QString> values)
{
    outboundDataTableModel.setDesiredOutboundValues(values);
}

void MainWindowController::loadConfiguration(const QString& configFilePath, bool initialLoad)
{
    if(!FileOperationsHandler::verifyFileSelection(configFilePath))
    {
        if(!configFilePath.isEmpty())
        {
            const QString msg = QString("%1").arg("The configuration file '" + configFilePath + "' is invalid or does not exist.");
            FileOperationsHandler::showFileErrorPopup("Configuration Error", msg);
        }
        return;
    }

    configurationLoaded = false;
    if(!initialLoad)
    {
        commsManager->stopStartTransmissionTimer(false);
    }

    if(!sds->loadSystemConfiguration(configFilePath))
    {
        const QString msg = QString("%1").arg("Error encountered while attempting to open configuration file '" + configFilePath + "'.");
        FileOperationsHandler::showFileErrorPopup("Configuration Error", msg);
        return;
    }

    if(!initialLoad)
    {
        commsManager->stopStartTransmissionTimer(true);
    }

    inboundDataTableModel.setInboundDataItems();
    outboundDataTableModel.setOutboundDataItems();
    configurationLoaded = true;
    sds->setSystemConfigFilePath(configFilePath);

    commsManager->setSocketPort(sds->getSocketPort());
    commsManager->setTransmissionPeriodicity(sds->getTransmissionPeriodicity());

    emit sendStatusBarMessage(commsManager->getConnectionStatusMsg());
    emit requestMainWindowUpdate();
}

void MainWindowController::performInitialSetup()
{
    if(sds->getSocketPort() > 0U &&
       sds->getTransmissionPeriodicity() > 0U &&
       configurationLoaded &&
       settingsManager->getAutoConnectSetting())
    {
        executeConnect();
    }
    else
    {
        emit sendStatusBarMessage("Not Connected");
    }

    commsManager->setConnectionNotificationEnable(settingsManager->getShowConnectionNotificationsSetting());
    emit notifyStatusChange();
}

void MainWindowController::executeConnect()
{
    emit sendStatusBarMessage("Connecting...");
    commsManager->connectToServer();
}

void MainWindowController::executeDisconnect()
{
    emit sendStatusBarMessage("Disconnecting...");
    commsManager->disconnectFromServer();
}

void MainWindowController::showUserActionErrorPopup(const QString& title, const QString& msg)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText("<p align='center'>" + msg + "</p>");
    msgBox.setFont(QFont("Segoe UI", 10));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void MainWindowController::validateOutboundCommand(const QString& dataItemName, QString& input)
{
    if(rangeCheckHandler->validateOutboundDataItem(dataItemName, input))
    {
        sds->setOutboundDisplayValue(dataItemName, input);

        emit outboundDataTableModel.layoutChanged();
        emit notifyDataUpdated();
    }
}
