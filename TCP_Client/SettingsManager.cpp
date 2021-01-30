#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include "SettingsManager.h"

// String identifiers for the persistent setting keys in the .ini file
const QString FileSelectionPathIdentifier = "File Selection Path";
const QString AutoConnectIdentifier = "Auto Connect";
const QString MainWindowSizeIdentifier = "Main Window Size";
const QString DisplayConnectionNotificationsIdentifier = "Display Connection Notifications";

SettingsManager::SettingsManager()
{
    configureSettingsDirectory();
    persistentSettings = std::make_unique<QSettings>(settingsPath + "/TCP_Client.ini", QSettings::IniFormat);
}

SettingsManager::~SettingsManager() = default;

QString SettingsManager::getFileSelectionPathSetting() const
{
    return (persistentSettings != nullptr) ? persistentSettings->value(FileSelectionPathIdentifier).toString() : QString();
}

bool SettingsManager::getAutoConnectSetting() const
{
    return (persistentSettings != nullptr) ? persistentSettings->value(AutoConnectIdentifier).toBool() : false;
}

QSize SettingsManager::getMainWindowSizeSetting() const
{
    return (persistentSettings != nullptr) ? persistentSettings->value(MainWindowSizeIdentifier).toSize() : QSize();
}

bool SettingsManager::getShowConnectionNotificationsSetting() const
{
    return (persistentSettings != nullptr) ? persistentSettings->value(DisplayConnectionNotificationsIdentifier).toBool() : false;
}

void SettingsManager::setFileSelectionPathSetting(const QString& path)
{
    if(persistentSettings != nullptr)
    {
        persistentSettings->setValue(FileSelectionPathIdentifier, path);
        persistentSettings->sync();
    }
}

void SettingsManager::setAutoConnectSetting(bool enabled)
{
    if(persistentSettings != nullptr)
    {
        persistentSettings->setValue(AutoConnectIdentifier, enabled);
        persistentSettings->sync();
    }
}

void SettingsManager::setMainWindowSizeSetting(const QSize& size)
{
    if(persistentSettings != nullptr)
    {
        persistentSettings->setValue(MainWindowSizeIdentifier, size);
        persistentSettings->sync();
    }
}

void SettingsManager::setShowConnectionNotificationsSetting(bool enabled)
{
    if(persistentSettings != nullptr)
    {
        persistentSettings->setValue(DisplayConnectionNotificationsIdentifier, enabled);
        persistentSettings->sync();
    }
}

void SettingsManager::configureSettingsDirectory()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    settingsPath = dir.path();
    if(!dir.exists()) dir.mkpath(settingsPath);
}
