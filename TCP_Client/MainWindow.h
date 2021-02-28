#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QTimer>

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
    void onActionSaveControlDataToFileTriggered();
    void onActionRestoreControlDataFromFileTriggered();
    void onActionLaunchDragon();
    void onActionRestoreAllDefaultsTriggered();
    void onActionRestoreSelectedDefaultsTriggered();
    void onButtonApplyClicked();
    void onActionClearSelectionTriggered();
    void onActionClearAllTriggered();
    void controlDataTableDoubleClicked(const QModelIndex& index);
    void onPushButtonMode1Clicked();
    void onPushButtonMode2Clicked();
    void onPushButtonOpenClicked();
    void onPushButtonCloseClicked();
    void onPushButtonEnableClicked();
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

    void blinkFaultIndicator();

private:
    Ui::MainWindow *ui;
    QMenu* fileActionMenu;
    QMenu* restoreDefaultsMenu;
    QMenu* clearMenu;
    QAction* restoreControlDataFromFileAction;
    QAction* saveControlDataToFileAction;
    QAction* restoreAllDefaultsAction;
    QAction* restoreSelectedDefaultsAction;
    QAction* clearSelectionAction;
    QAction* clearAllAction;
    QLabel* statusBarLabel;
    std::unique_ptr<MainWindowController> mainWindowController;
    QTimer faultIndicatorBlinkTimer;
    bool faultIndicatorIsOn = false;

    void configureInboundDataTableView();
    void configureOutboundDataTableView();
    void configureCommonTableSettings(QTableView* table);
    void setupStatusBar();
    void setupToolButtons();
    void updateMonitoringAndControlItems();
};
#endif // MAINWINDOW_H
