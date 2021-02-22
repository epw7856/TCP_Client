#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

class MainWindowController;
class QLabel;
class QTableView;

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
    void refreshDataDisplays();
    void periodicUpdate();
    void receivedUpdateRequestFromController();

private slots:
    void actionViewApplicationInformation();
    void onActionLoadSystemConfigurationFileTriggered();
    void onActionViewApplicationSettingsTriggered();
    void onActionConnectToServerTriggered();
    void onActionDisconnectFromServerTriggered();
    void onActionSaveControlDataToFileClicked();
    void onActionRestoreControlDataFromFileClicked();
    void onActionLaunchDragon();
    void onButtonResetClicked();
    void onButtonApplyClicked();
    void onActionClearSelectionTriggered();
    void onActionClearAllTriggered();
    void controlDataTableDoubleClicked(const QModelIndex& index);
    void onPushButtonMode1Clicked();
    void onPushButtonMode2Clicked();
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::MainWindow *ui;
    QMenu* fileActionMenu;
    QMenu* clearMenu;
    QAction* restoreControlDataFromFileAction;
    QAction* saveControlDataToFileAction;
    QAction* clearSelectionAction;
    QAction* clearAllAction;
    QLabel* statusBarLabel;
    std::unique_ptr<MainWindowController> mainWindowController;

    void configureInboundDataTableView();
    void configureOutboundDataTableView();
    void configureCommonTableSettings(QTableView* table);
    void setupStatusBar();
    void updateMonitoringAndControlItems();
};
#endif // MAINWINDOW_H
