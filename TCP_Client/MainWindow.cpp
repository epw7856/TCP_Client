#include "MainWindow.h"
#include "MainWindowController.h"
#include <QLabel>
#include <QShowEvent>
#include "ui_MainWindow.h"

MainWindow::MainWindow(const QString& configFilePathArg, QWidget *parent)
:
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mainWindowController(std::make_unique<MainWindowController>(configFilePathArg))
{
    ui->setupUi(this);

    ui->labelHeaderText->setText(mainWindowController->getHeaderFooterText());
    ui->labelFooterText->setText(mainWindowController->getHeaderFooterText());

    QFont font("Segoe UI", 10);

    // Setup the File Actions toolbutton and the associated dropdown menu actions
    saveControlDataToFileAction = new QAction("Save Control Data To File", this);
    saveControlDataToFileAction->setFont(font);

    restoreControlDataFromFileAction = new QAction("Restore Control Data From File", this);
    restoreControlDataFromFileAction->setFont(font);

    fileActionMenu = new QMenu(this);
    fileActionMenu->addAction(saveControlDataToFileAction);
    fileActionMenu->addAction(restoreControlDataFromFileAction);

    ui->toolButtonFileActions->setMenu(fileActionMenu);
    ui->toolButtonFileActions->setPopupMode(QToolButton::InstantPopup);

    // Setup the Clear toolbutton and the associated dropdown menu actions
    clearSelectionAction = new QAction("Clear Selected Data Items", this);
    clearSelectionAction->setFont(font);

    clearAllAction = new QAction("Clear All Data Items", this);
    clearAllAction->setFont(font);

    clearMenu = new QMenu(this);
    clearMenu->addAction(clearAllAction);
    clearMenu->addAction(clearSelectionAction);

    ui->toolButtonClear->setMenu(clearMenu);
    ui->toolButtonClear->setPopupMode(QToolButton::InstantPopup);

    // Setup the tables
    configureInboundDataTableView();
    configureOutboundDataTableView();

    // Setup the bottom status bar
    setupStatusBar();

    // Connections with the menu bar in the UI
    connect(ui->actionViewApplicationInformation, &QAction::triggered, this, &MainWindow::actionViewApplicationInformation);
    connect(ui->actionLoadSystemConfigurationFile, &QAction::triggered, this, &MainWindow::onActionLoadSystemConfigurationFileTriggered);
    connect(ui->actionViewApplicationSettings, &QAction::triggered, this, &MainWindow::onActionViewApplicationSettingsTriggered);
    connect(ui->actionConnectToServer, &QAction::triggered, this, &MainWindow::onActionConnectToServerTriggered);
    connect(ui->actionDisconnectFromServer, &QAction::triggered, this, &MainWindow::onActionDisconnectFromServerTriggered);
    connect(ui->actionLaunchDragon, &QAction::triggered, this, &MainWindow::onActionLaunchDragon);

    // Connections with the pushbuttons in the UI
    connect(ui->pushButtonReset, &QPushButton::clicked, this, &MainWindow::onButtonResetClicked);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &MainWindow::onButtonApplyClicked);
    connect(saveControlDataToFileAction, &QAction::triggered, this, &MainWindow::onActionSaveControlDataToFileClicked);
    connect(restoreControlDataFromFileAction, &QAction::triggered, this, &MainWindow::onActionRestoreControlDataFromFileClicked);
    connect(clearSelectionAction, &QAction::triggered, this, &MainWindow::onActionClearSelectionTriggered);
    connect(clearAllAction, &QAction::triggered, this, &MainWindow::onActionClearAllTriggered);

    // Connections from MainWindowController to MainWindow
    connect(mainWindowController.get(), &MainWindowController::sendStatusBarMessage, this, &MainWindow::showStatusBarMessage);
    connect(mainWindowController.get(), &MainWindowController::notifyInboundDataUpdated, this, &MainWindow::refreshInboundDataDisplays);
    connect(mainWindowController.get(), &MainWindowController::notifyStatusChange, this, &MainWindow::periodicUpdate);
    connect(mainWindowController.get(), &MainWindowController::requestMainWindowUpdate, this, &MainWindow::receivedUpdateRequestFromController);

    // Connections from user interaction with the Control Data table
    connect(ui->tableViewControlData, &QTableView::doubleClicked, this, &MainWindow::controlDataTableDoubleClicked);

    // Perform initial setup and refresh the UI
    mainWindowController->performInitialSetup();
    periodicUpdate();
    updateMonitoringAndControlItems();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    // Restore size settings
    QSize sizeSettings = mainWindowController->getMainWindowSizeSetting();
    bool isMaximized = mainWindowController->getMainWindowMaximizedSetting();
    if(isMaximized)
    {
        showMaximized();
    }
    else if(sizeSettings != QSize())
    {
        resize(sizeSettings);
    }

    // Configure each column within the status and control data tables to be resizable
    ui->tableViewStatusData->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableViewStatusData->horizontalHeader()->setStretchLastSection(true);

    ui->tableViewControlData->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableViewControlData->horizontalHeader()->setStretchLastSection(true);

    event->accept();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    mainWindowController->saveMainWindowSizeSetting({width(), height()});
    mainWindowController->saveMainWindowMaximizedSetting(isMaximized());
    event->accept();
}

void MainWindow::setupStatusBar()
{
    statusBarLabel = new QLabel();
    statusBarLabel->setFont(QFont("Segoe UI", 10));
    ui->statusBar->setStyleSheet("QStatusBar{border-top: 1px outset grey;}");
    ui->statusBar->addPermanentWidget(statusBarLabel);
}

void MainWindow::updateMonitoringAndControlItems()
{
    ui->labelMaxTempIndiv->setText(mainWindowController->getMaxTempIndiv());
    ui->labelMaxTempGroup->setText(mainWindowController->getMaxTempGroup());
    ui->labelMinTempIndiv->setText(mainWindowController->getMinTempIndiv());
    ui->labelMinTempGroup->setText(mainWindowController->getMinTempGroup());
    ui->labelSetPoint1->setText(mainWindowController->getSetPoint1());
    ui->labelSetPoint2->setText(mainWindowController->getSetPoint2());
    ui->labelCurrentMode->setText(mainWindowController->getCurrentMode());
    ui->labelCommandedMode->setText(mainWindowController->getCommandedMode());
}

void MainWindow::showStatusBarMessage(QString msg)
{
    statusBarLabel->clear();
    statusBarLabel->setText("Status: " + msg + "  ");
}

void MainWindow::refreshInboundDataDisplays()
{
    ui->tableViewStatusData->update();
    updateMonitoringAndControlItems();
}

void MainWindow::periodicUpdate()
{
    ui->toolButtonFileActions->setEnabled(mainWindowController->enableFileActionsButton());
    ui->actionConnectToServer->setEnabled(mainWindowController->enableActionConnectToServer());
    ui->actionDisconnectFromServer->setEnabled(mainWindowController->enableActionDisconnectFromServer());
    ui->toolButtonClear->setEnabled(mainWindowController->enableClearButton());
    ui->pushButtonReset->setEnabled(mainWindowController->enableResetButton());
}

void MainWindow::receivedUpdateRequestFromController()
{
    ui->labelHeaderText->setText(mainWindowController->getHeaderFooterText());
    ui->labelFooterText->setText(mainWindowController->getHeaderFooterText());
}

void MainWindow::actionViewApplicationInformation()
{
    mainWindowController->showAboutDialog(this);
}

void MainWindow::onActionLoadSystemConfigurationFileTriggered()
{
    mainWindowController->selectConfigurationFile(this);
}

void MainWindow::onActionViewApplicationSettingsTriggered()
{
    mainWindowController->showApplicationSettingsDialog(this);
}

void MainWindow::onActionConnectToServerTriggered()
{
    mainWindowController->requestConnectToServer();
}

void MainWindow::onActionDisconnectFromServerTriggered()
{
    mainWindowController->requestDisconnectFromServer();
}

void MainWindow::onActionSaveControlDataToFileClicked()
{

}

void MainWindow::onActionRestoreControlDataFromFileClicked()
{

}

void MainWindow::onActionLaunchDragon()
{

}

void MainWindow::onButtonResetClicked()
{
    mainWindowController->resetDesiredOutboundValuesToDefaults();
}

void MainWindow::onButtonApplyClicked()
{
    mainWindowController->applyDesiredOutboundValues();
}

void MainWindow::onActionClearSelectionTriggered()
{
    mainWindowController->resetSelectedDesiredOutboundValuesToDefaults(ui->tableViewControlData->selectionModel()->selectedRows());
}

void MainWindow::onActionClearAllTriggered()
{
    mainWindowController->clearDesiredOutboundValues();
}

void MainWindow::controlDataTableDoubleClicked(const QModelIndex& index)
{
    mainWindowController->outboundTableDoubleClicked(index);
}

void MainWindow::configureInboundDataTableView()
{
    // Add table model data and configure settings
    ui->tableViewStatusData->setModel(&(mainWindowController->getInboundDataTableModel()));
    ui->tableViewStatusData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    configureCommonTableSettings(ui->tableViewStatusData);
}

void MainWindow::configureOutboundDataTableView()
{
    // Add table model data and configure settings
    ui->tableViewControlData->setModel(&(mainWindowController->getOutboundDataTableModel()));
    configureCommonTableSettings(ui->tableViewControlData);
}

void MainWindow::configureCommonTableSettings(QTableView* table)
{
    // Hide the index column and set selection/focus behavior
    table->setColumnHidden(0, true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::MultiSelection);
    table->setFocusPolicy(Qt::NoFocus);

    // Set font styles
    QFont font(table->font());
    font.setBold(true);
    table->horizontalHeader()->setFont(font);
    table->setStyleSheet("QHeaderView::section { background-color: rgb(240, 240, 240) }\n"
                         "QTableView::item:selected {background-color: #3399FF; color: white;}");

    // Configure the horizontal header settings
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setFixedHeight(25);
    table->horizontalHeader()->setSectionsClickable(false);

    // Configure the vertical header settings
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(20);
    table->verticalHeader()->hide();
}
