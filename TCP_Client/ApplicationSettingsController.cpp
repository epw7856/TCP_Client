#include "ApplicationInterface.h"
#include "ApplicationSettingsController.h"
#include <QFileInfo>
#include <QMessageBox>
#include "SettingsInterface.h"

ApplicationSettingsController::ApplicationSettingsController
(
    ApplicationInterface& localAppInterface,
    SettingsInterface& localSettingsInterface
)
:
    appInterface(localAppInterface),
    settingsInterface(localSettingsInterface)
{

}

QString ApplicationSettingsController::getCurrentConfig() const
{
    QFileInfo file(appInterface.getSystemConfigFilePath());
    return (file.fileName().isEmpty()) ? "No configuration file loaded" : "\"" + file.fileName() + "\"";
}

unsigned ApplicationSettingsController::getSocketPort() const
{
    return appInterface.getSocketPort();
}

unsigned ApplicationSettingsController::getTransmissionPeriodicity() const
{
    return appInterface.getTransmissionPeriodicity();
}

bool ApplicationSettingsController::isShowConnectionNotificationsEnabled() const
{
    return settingsInterface.getShowConnectionNotificationsSetting();
}

bool ApplicationSettingsController::isAutoConnectEnabled() const
{
    return settingsInterface.getAutoConnectSetting();
}

bool ApplicationSettingsController::verifyUserInput(const QString& socketPort,
                                                    const QString& transmissionPeriodicity)
{
    bool status = true;
    if(!verifyInputValue(socketPort))
    {
        showUserInputErrorPopup("Socket Port Error",
                                 "Please enter a value socket port greater than 0.");
        status = false;
    }
    else if(!verifyInputValue(transmissionPeriodicity))
    {
        showUserInputErrorPopup("Transmission Periodicity Error",
                                 "Please enter a transmission periodicity in milliseconds greater than 0ms.");
        status = false;
    }

    return status;
}

bool ApplicationSettingsController::setSocketPort(const QString& input)
{
    bool requireReconnect = input.toUInt() != appInterface.getSocketPort();
    appInterface.setSocketPort(input.toUInt());
    return requireReconnect;
}

void ApplicationSettingsController::setTransmissionPeriodicity(const QString& input)
{
    appInterface.setTransmissionPeriodicity(input.toUInt());
}

void ApplicationSettingsController::setShowConnectionNotificationsEnabled(bool enabled)
{
    settingsInterface.setShowConnectionNotificationsSetting(enabled);
}

void ApplicationSettingsController::setAutoConnectEnabled(bool enabled)
{
    settingsInterface.setAutoConnectSetting(enabled);
}

bool ApplicationSettingsController::verifyInputValue(const QString& input)
{
    bool status;
    unsigned value = input.toUInt(&status);

    return (status && value > 0U);
}

void ApplicationSettingsController::showUserInputErrorPopup(const QString& title, const QString& msg)
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
