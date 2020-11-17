#include "CommunicationsManager.h"
#include "ConfigFileVerificationHandler.h"
#include "MainWindowController.h"
#include "SystemDataSource.h"

MainWindowController::MainWindowController(const QString& configFilePath)
:
    sds(std::make_unique<SystemDataSource>()),
    verifier(std::make_unique<ConfigFileVerificationHandler>())
{
    // Connections from MainWindowController to CommunicationsManager
    connect(this, &MainWindowController::requestDisconnectFromServer, commsManager.get(), &CommunicationsManager::requestDisconnectFromServer);
    connect(this, &MainWindowController::requestConnectToServer, commsManager.get(), &CommunicationsManager::requestConnectToServer);

    // Connections from CommunicationsManager to MainWindowController
    connect(commsManager.get(), &CommunicationsManager::notifyConnectedToServer, this, &MainWindowController::receivedConnectedNotification);
    connect(commsManager.get(), &CommunicationsManager::notifyDisconnectedFromServer, this, &MainWindowController::receivedDisconnectedNotification);

    if(!configFilePath.isEmpty())
    {
        loadConfiguration(configFilePath);
    }

    commsManager = std::make_unique<CommunicationsManager>(sds->getSocketPort());
    commsManager->moveToThread(&commsThread);
    commsThread.start();
}

MainWindowController::~MainWindowController()
{
    emit requestDisconnectFromServer();
    commsThread.quit();
    commsThread.wait(2000);
}

void MainWindowController::receivedConnectedNotification()
{
    isConnectedToServer = true;
}

void MainWindowController::receivedDisconnectedNotification()
{
    isConnectedToServer = false;
}

void MainWindowController::loadConfiguration(const QString& configFilePath)
{
    if(!verifier->verifyFilePath(configFilePath))
    {
        return;
    }

    if(!sds->loadSystemConfig(configFilePath))
    {
        const QString msg = QString("%1").arg("Error encountered while attempting to open configuration file '" + configFilePath + "'.");
        verifier->showConfigFileErrorPopup(msg);
    }
}
