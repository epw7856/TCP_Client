#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <memory>
#include <QSize>
#include <QString>
#include "SettingsInterface.h"

class QSettings;

class SettingsManager : public SettingsInterface
{
public:
    SettingsManager();
    ~SettingsManager() override;

    bool getAutoConnectSetting() const override;
    QSize getMainWindowSizeSetting() const override;
    bool getMainWindowMaximizedSetting() const override;
    bool getShowConnectionNotificationsSetting() const override;

    void setAutoConnectSetting(bool enabled) override;
    void setMainWindowSizeSetting(const QSize& size) override;
    void setMainWindowMaximizedSetting(bool maximized) override;
    void setShowConnectionNotificationsSetting(bool enabled) override;

private:
    std::unique_ptr<QSettings> persistentSettings;
    QString settingsPath= "";

    void configureSettingsDirectory();
};

#endif // SETTINGSMANAGER_H
