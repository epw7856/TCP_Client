#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <memory>
#include <QObject>

class SystemDataSource;

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController(const QString& configFilePath);
    ~MainWindowController();

private:
    std::unique_ptr<SystemDataSource> sds;

    void loadConfiguration(const QString& configFilePath);
    void showConfigFileErrorPopup(const QString& message);
};

#endif // MAINWINDOWCONTROLLER_H
