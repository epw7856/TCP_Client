#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include "SettingsManager.h"

// String identifiers for the persistent setting keys in the .ini file
static const QString FileSelectionPathIdentifier = "File Selection Path";
static const QString AutoConnectIdentifier = "Auto Connect";
static const QString MainWindowSizeIdentifier = "Main Window Size";
static const QString DisplayConnectionNotificationsIdentifier = "Display Connection Notifications";

SettingsManager::SettingsManager()
{
    if(configureSettingsDirectory())
    {
        persistentSettings = std::make_unique<QSettings>(settingsPath + "/TCP_Client.ini", QSettings::IniFormat);
    }
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

bool SettingsManager::getDisplayConnectionNotificationsSetting() const
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

void SettingsManager::setDisplayConnectionNotificationsSetting(bool enabled)
{
    if(persistentSettings != nullptr)
    {
        persistentSettings->setValue(DisplayConnectionNotificationsIdentifier, enabled);
        persistentSettings->sync();
    }
}

bool SettingsManager::configureSettingsDirectory()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    return (dir.exists()) ? true : dir.mkpath(settingsPath);
}
