#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

class MainWindowController;
class QString;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& configFilePathArg, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onActionAboutTriggered();
    void onActionExitTriggered();
    void onActionLoadSystemConfigurationFileTriggered();
    void onActionViewApplicationConfigurationTriggered();
    void onActionConnectToServerTriggered();
    void onActionDisconnectFromServerTriggered();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<MainWindowController> mainWindowController;
};
#endif // MAINWINDOW_H
