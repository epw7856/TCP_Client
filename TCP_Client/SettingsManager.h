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

    QString getFileSelectionPathSetting() const override;
    bool getAutoConnectSetting() const override;
    QSize getMainWindowSizeSetting() const override;
    bool getDisplayConnectionNotificationsSetting() const override;

    void setFileSelectionPathSetting(const QString& path) override;
    void setAutoConnectSetting(bool enabled) override;
    void setMainWindowSizeSetting(const QSize& size) override;
    void setDisplayConnectionNotificationsSetting(bool enabled) override;

private:
    std::unique_ptr<QSettings> persistentSettings;
    QString settingsPath= "";

    bool configureSettingsDirectory();
};

#endif // SETTINGSMANAGER_H