#include "CommunicationsManager.h"
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
    outboundDataTableModel(*sds)
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

bool MainWindowController::enableActionConnectToServer() const
{
    return (commsManager->getConnectionStatus() == ConnectionStatus::Unconnected);
}

bool MainWindowController::enableActionDisconnectFromServer() const
{
    return (commsManager->getConnectionStatus() == ConnectionStatus::Connected);
}

bool MainWindowController::enableButtonSaveToFile() const
{
    return configurationLoaded;
}

bool MainWindowController::enableButtonRestoreFromFile() const
{
    return configurationLoaded;
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
            emit sendStatusBarMessage("Ready");
        }
    }

    commsManager->setConnectionNotificationEnable(true);
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
