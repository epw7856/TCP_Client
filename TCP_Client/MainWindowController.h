#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <memory>
#include <QObject>

class CommunicationsManager;
class ConfigFileVerificationHandler;
class SystemDataSource;

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController(const QString& configFilePath);
    ~MainWindowController();

private:
    std::unique_ptr<SystemDataSource> sds;
    std::unique_ptr<ConfigFileVerificationHandler> verifier;
    std::unique_ptr<CommunicationsManager> commsManager;

    void loadConfiguration(const QString& configFilePath);
};

#endif // MAINWINDOWCONTROLLER_H
