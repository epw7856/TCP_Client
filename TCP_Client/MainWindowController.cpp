#include "AboutDialog.h"
#include "ApplicationSettingsDialog.h"
#include "CommunicationsManager.h"
#include "DataItem.h"
#include "EnumSelectionDialog.h"
#include "FileOperationsHandler.h"
#include "MainWindowController.h"
#include <QMessageBox>
#include "SettingsManager.h"
#include "SystemDataSource.h"

MainWindowController::MainWindowController(const QString& configFilePath)
:
    sds(std::make_unique<SystemDataSource>()),
    settingsManager(std::make_unique<SettingsManager>()),
    inboundDataTableModel(*sds),
    outboundDataTableModel(*sds, *sds)
{
    if(!configFilePath.isEmpty())
    {
        loadConfiguration(configFilePath, true);
    }

    commsManager = std::make_unique<CommunicationsManager>(*sds, *sds);

    // Connections from CommunicationsManager to MainWindowController
    connect(commsManager.get(), &CommunicationsManager::sendStatusUpdate, this, &MainWindowController::receivedStatusUpdate);
    connect(commsManager.get(), &CommunicationsManager::inboundDataUpdated, this, &MainWindowController::notifyInboundDataUpdated);
}

QString MainWindowController::getHeaderFooterText() const
{
    return sds->getHeaderFooterText();
}

MainWindowController::~MainWindowController() = default;

void MainWindowController::requestConnectToServer()
{
    if(sds->getSocketPort() <= 0U)
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
        status = FileOperationsHandler::showFileSelectionWarning();
    }

    if(status)
    {
        const QString configFile = FileOperationsHandler::showFileSelectionDialog(parent);
        loadConfiguration(configFile, false);
    }
}

bool MainWindowController::enableFileActionsButton() const
{
    return configurationLoaded;
}

bool MainWindowController::enableActionConnectToServer() const
{
    return (commsManager->getConnectionStatus() == ConnectionStatus::Unconnected);
}

bool MainWindowController::enableActionDisconnectFromServer() const
{
    return (commsManager->getConnectionStatus() == ConnectionStatus::Connected);
}

bool MainWindowController::enableClearButton() const
{
    return configurationLoaded;
}

bool MainWindowController::enableResetButton() const
{
    return configurationLoaded;
}

void MainWindowController::outboundTableDoubleClicked(const QModelIndex& index)
{
    if(index.column() == outboundDataTableModel.getNewValueColumn())
    {
        DataItem* item = sds->getOutboundDataItem(outboundDataTableModel.index(index.row(), 0).data().toInt());
        if(item->isEnumType())
        {
            EnumSelectionDialog enumDialog;
            enumDialog.setDataItemName(item->getDataItemName());
            enumDialog.setEnumName(item->getDataItemType());
            enumDialog.setEnumStrings(sds->getEnumStrings(item->getDataItemType()));
            enumDialog.setCurrentValue(item->getDisplayValue());
            enumDialog.exec();

            if(enumDialog.isAccepted())
            {
                outboundDataTableModel.setData(index, enumDialog.getNewValue(), Qt::EditRole);
            }
        }
    }
}

void MainWindowController::applyDesiredOutboundValues()
{
    outboundDataTableModel.applyDesiredOutboundValues();
}

void MainWindowController::clearDesiredOutboundValues()
{
    outboundDataTableModel.clearDesiredOutboundValues();
}

void MainWindowController::resetDesiredOutboundValuesToDefaults()
{
    outboundDataTableModel.resetDesiredOutboundValuesToDefaults();
}

void MainWindowController::resetSelectedDesiredOutboundValuesToDefaults(const QModelIndexList& selection)
{
    std::vector<unsigned> indices;
    for(const auto& i : selection)
    {
        indices.push_back(outboundDataTableModel.index(i.row(), 0).data().toInt());
    }

    outboundDataTableModel.clearDesiredOutboundValues(indices);
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

void MainWindowController::updateInboundDataDisplay()
{
    emit notifyInboundDataUpdated();
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

    if(reconnect)
    {
       if(commsManager->getConnectionStatus() == ConnectionStatus::Connected)
       {
           executeDisconnect();
       }
       executeConnect();
    }
}

void MainWindowController::loadConfiguration(const QString& configFilePath, bool initialLoad)
{
    if(!FileOperationsHandler::verifyFilePath(configFilePath))
    {
        if(!configFilePath.isEmpty())
        {
            const QString msg = QString("%1").arg("The configuration file '" + configFilePath + "' is invalid or does not exist.");
            FileOperationsHandler::showConfigFileErrorPopup(msg);
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
        FileOperationsHandler::showConfigFileErrorPopup(msg);
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
}

void MainWindowController::performInitialSetup()
{
    if(sds->getSocketPort() > 0U)
    {
        commsManager->setSocketPort(sds->getSocketPort());
        commsManager->setTransmissionPeriodicity(sds->getTransmissionPeriodicity());

        if(configurationLoaded && settingsManager->getAutoConnectSetting())
        {
            executeConnect();
        }
        else
        {
            emit sendStatusBarMessage("Not Connected");
        }
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
