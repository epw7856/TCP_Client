#include "ApplicationInterface.h"
#include "ApplicationSettingsController.h"
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
    return appInterface.getSystemConfigFilePath();
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
