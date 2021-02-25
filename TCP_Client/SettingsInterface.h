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
    virtual bool getAutoConnectSetting() const = 0;
    virtual QSize getMainWindowSizeSetting() const = 0;
    virtual bool getMainWindowMaximizedSetting() const = 0;
    virtual bool getShowConnectionNotificationsSetting() const = 0;
    virtual QString getFileSelectionPath() const = 0;

    virtual void setAutoConnectSetting(bool enabled) = 0;
    virtual void setMainWindowSizeSetting(const QSize& size) = 0;
    virtual void setMainWindowMaximizedSetting(bool maximized) = 0;
    virtual void setShowConnectionNotificationsSetting(bool enabled) = 0;
    virtual void setFileSelectionPath(const QString& path) = 0;
};

#endif // SETTINGSINTERFACE_H
