#include "CommunicationsManager.h"
#include "ConfigFileVerificationHandler.h"
#include "MainWindowController.h"
#include "SystemDataSource.h"

MainWindowController::MainWindowController(const QString& configFilePath)
:
    sds(std::make_unique<SystemDataSource>()),
    verifier(std::make_unique<ConfigFileVerificationHandler>())
{
    if(!configFilePath.isEmpty())
    {
        loadConfiguration(configFilePath);
    }

    commsManager = std::make_unique<CommunicationsManager>(sds->getSocketPort());
}

MainWindowController::~MainWindowController() = default;

void MainWindowController::loadConfiguration(const QString& configFilePath)
{
    if(!verifier->verifyFilePath(configFilePath))
    {
        return;
    }

    if(!sds->loadSystemConfiguration(configFilePath))
    {
        const QString msg = QString("%1").arg("Error encountered while attempting to open configuration file '" + configFilePath + "'.");
        verifier->showConfigFileErrorPopup(msg);
    }
}
