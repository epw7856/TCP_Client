#ifndef SETTINGSINTERFACE_H
#define SETTINGSINTERFACE_H

#include <QSize>
#include <QString>

class SettingsInterface
{
public:
    SettingsInterface() {}
    virtual ~SettingsInterface() = default;

    // Persistent application settings API
    virtual QString getFileSelectionPathSetting() const = 0;
    virtual bool getAutoConnectionSetting() const = 0;
    virtual QSize getMainWindowSizeSetting() const = 0;
    virtual bool getDisplayConnectionNotificationsSetting() const = 0;

    virtual void setFileSelectionPathSetting(const QString& path) = 0;
    virtual void setAutoConnectionSetting(bool enabled) = 0;
    virtual void setMainWindowSizeSetting(const QSize& size) = 0;
    virtual void setDisplayConnectionNotificationsSetting(bool enabled) = 0;
};

#endif // SETTINGSINTERFACE_H
