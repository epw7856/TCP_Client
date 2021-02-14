#ifndef APPLICATIONSETTINGSCONTROLLER_H
#define APPLICATIONSETTINGSCONTROLLER_H

#include <QString>

class ApplicationInterface;
class SettingsInterface;

class ApplicationSettingsController
{
public:
    ApplicationSettingsController(ApplicationInterface& localAppInterface,
                                  SettingsInterface& localSettingsInterface);

    QString getCurrentConfig() const;
    unsigned getSocketPort() const;
    unsigned getTransmissionPeriodicity() const;
    bool isShowConnectionNotificationsEnabled() const;
    bool isAutoConnectEnabled() const;

private:
    ApplicationInterface& appInterface;
    SettingsInterface& settingsInterface;
};

#endif // APPLICATIONSETTINGSCONTROLLER_H
