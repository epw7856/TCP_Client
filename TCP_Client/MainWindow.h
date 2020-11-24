#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "InboundDataTableModel.h"
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

private slots:
    void onActionAboutTriggered();
    void onActionExitTriggered();
    void onActionLoadSystemConfigurationFileTriggered();
    void onActionViewApplicationConfigurationTriggered();
    void onActionConnectToServerTriggered();
    void onActionDisconnectFromServerTriggered();
    void onButtonSaveToFileClicked();
    void onButtonRestoreFromFileClicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<MainWindowController> mainWindowController;
    InboundDataTableModel inboundDataTableModel;
    std::unique_ptr<QLabel> statusBarLabel;

    void configureInboundDataTableView();
    void setupStatusBar();
};
#endif // MAINWINDOW_H
