#include "MainWindowController.h"
#include "SystemDataSource.h"

MainWindowController::MainWindowController(const QString& configFilePath)
:
    sds(std::make_unique<SystemDataSource>())
{
    // Verify config file path
    if(!sds->loadSystemConfig(configFilePath))
    {
        // Call function to show config file error popup
    }
}

MainWindowController::~MainWindowController() = default;
