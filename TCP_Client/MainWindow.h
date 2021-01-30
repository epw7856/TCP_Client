#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

class MainWindowController;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& configFilePathArg, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showStatusBarMessage(QString msg);
    void refreshStatusDataDisplay();
    void periodicUpdate();

private slots:
    void onActionAboutTriggered();
    void onActionExitTriggered();
    void onActionLoadSystemConfigurationFileTriggered();
    void onActionViewApplicationConfigurationTriggered();
    void onActionConnectToServerTriggered();
    void onActionDisconnectFromServerTriggered();
    void onButtonSaveToFileClicked();
    void onButtonRestoreFromFileClicked();
    void onButtonApplyClicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<MainWindowController> mainWindowController;
    std::unique_ptr<QLabel> statusBarLabel;

    void configureInboundDataTableView();
    void configureOutboundDataTableView();
    void setupStatusBar();
};
#endif // MAINWINDOW_H
