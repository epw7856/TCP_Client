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

    bool verifyUserInput(const QString& socketPort,
                         const QString& transmissionPeriodicity);

    bool setSocketPort(const QString& input);
    void setTransmissionPeriodicity(const QString& input);
    void setShowConnectionNotificationsEnabled(bool enabled);
    void setAutoConnectEnabled(bool enabled);

private:
    ApplicationInterface& appInterface;
    SettingsInterface& settingsInterface;

    bool verifyInputValue(const QString& input);
    void showUserInputErrorPopup(const QString& title, const QString& msg);
};

#endif // APPLICATIONSETTINGSCONTROLLER_H
