#include "MainWindowController.h"
#include <QFileInfo>
#include <QMessageBox>
#include "SystemDataSource.h"

MainWindowController::MainWindowController(const QString& configFilePath)
:
    sds(std::make_unique<SystemDataSource>())
{
    if(!configFilePath.isEmpty())
    {
        loadConfiguration(configFilePath);
    }
}

MainWindowController::~MainWindowController() = default;

void MainWindowController::loadConfiguration(const QString& configFilePath)
{
    // Verify config file path
    QFileInfo file(configFilePath);

    if(!file.exists() || (file.suffix() != "json"))
    {
        const QString msg = QString("%1").arg("The configuration file '" + configFilePath + "' is invalid or does not exist.");
        showConfigFileErrorPopup(msg);
        return;
    }

    if(!sds->loadSystemConfig(configFilePath))
    {
        const QString msg = QString("%1").arg("Error encountered while attempting to open configuration file '" + configFilePath + "'.");
        showConfigFileErrorPopup(msg);
    }
}

void MainWindowController::showConfigFileErrorPopup(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Configuration Error");
    msgBox.setText("<p align='center'>" + message + "</p>");
    msgBox.setFont(QFont("Segoe UI", 10));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}
