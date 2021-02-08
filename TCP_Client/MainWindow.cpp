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

    // Setup the Clear toolbutton and the associated dropdown menu actions
    clearSelectionAction = new QAction("Clear Selected Data Items", this);
    clearSelectionAction->setFont(QFont("Segoe UI", 10));

    clearAllAction = new QAction("Clear All Data Items", this);
    clearAllAction->setFont(QFont("Segoe UI", 10));

    clearMenu = new QMenu(this);
    clearMenu->addAction(clearSelectionAction);
    clearMenu->addAction(clearAllAction);

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
    connect(ui->actionViewApplicationConfiguration, &QAction::triggered, this, &MainWindow::onActionViewApplicationConfigurationTriggered);
    connect(ui->actionConnectToServer, &QAction::triggered, this, &MainWindow::onActionConnectToServerTriggered);
    connect(ui->actionDisconnectFromServer, &QAction::triggered, this, &MainWindow::onActionDisconnectFromServerTriggered);
    connect(ui->actionSaveStatusDataToFile, &QAction::triggered, this, &MainWindow::onActionSaveStatusDataToFileClicked);
    connect(ui->actionSaveControlDataToFile, &QAction::triggered, this, &MainWindow::onActionSaveControlDataToFileClicked);
    connect(ui->actionRestoreControlDataFromFile, &QAction::triggered, this, &MainWindow::onActionRestoreControlDataFromFileClicked);

    // Connections with the pushbuttons in the UI
    connect(ui->pushButtonReset, &QPushButton::clicked, this, &MainWindow::onButtonResetClicked);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &MainWindow::onButtonApplyClicked);
    connect(clearSelectionAction, &QAction::triggered, this, &MainWindow::onActionClearSelectionTriggered);
    connect(clearAllAction, &QAction::triggered, this, &MainWindow::onActionClearAllTriggered);

    // Connections from MainWindowController to MainWindow
    connect(mainWindowController.get(), &MainWindowController::sendStatusBarMessage, this, &MainWindow::showStatusBarMessage);
    connect(mainWindowController.get(), &MainWindowController::notifyInboundDataUpdated, this, &MainWindow::refreshStatusDataDisplay);
    connect(mainWindowController.get(), &MainWindowController::notifyStatusChange, this, &MainWindow::periodicUpdate);

    // Perform initial setup and refresh the UI
    mainWindowController->performInitialSetup();
    periodicUpdate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    // Configure each column within the status and control data tables to be resizable
    ui->tableViewStatusData->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableViewStatusData->horizontalHeader()->setStretchLastSection(true);

    ui->tableViewControlData->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableViewControlData->horizontalHeader()->setStretchLastSection(true);

    event->accept();
}

void MainWindow::setupStatusBar()
{
    statusBarLabel = new QLabel();
    statusBarLabel->setFont(QFont("Segoe UI", 10));
    ui->statusBar->setStyleSheet("QStatusBar{border-top: 1px outset grey;}");
    ui->statusBar->addPermanentWidget(statusBarLabel);
    ui->statusBar->setSizeGripEnabled(false);
}

void MainWindow::showStatusBarMessage(QString msg)
{
    statusBarLabel->clear();
    statusBarLabel->setText("Status: " + msg + "  ");
}

void MainWindow::refreshStatusDataDisplay()
{
    emit mainWindowController->getInboundDataTableModel().layoutChanged();
    ui->tableViewStatusData->update();
}

void MainWindow::periodicUpdate()
{
    ui->menuFileActions->setEnabled(mainWindowController->enableFileActionsMenu());
    ui->actionConnectToServer->setEnabled(mainWindowController->enableActionConnectToServer());
    ui->actionDisconnectFromServer->setEnabled(mainWindowController->enableActionDisconnectFromServer());
    ui->toolButtonClear->setEnabled(mainWindowController->enableClearButton());
    ui->pushButtonReset->setEnabled(mainWindowController->enableResetButton());
}

void MainWindow::actionViewApplicationInformation()
{
    mainWindowController->showAboutDialog(this);
}

void MainWindow::onActionLoadSystemConfigurationFileTriggered()
{
    mainWindowController->selectConfigurationFile(this);
}

void MainWindow::onActionViewApplicationConfigurationTriggered()
{

}

void MainWindow::onActionConnectToServerTriggered()
{
    mainWindowController->requestConnectToServer();
}

void MainWindow::onActionDisconnectFromServerTriggered()
{
    mainWindowController->requestDisconnectFromServer();
}

void MainWindow::onActionSaveStatusDataToFileClicked()
{

}

void MainWindow::onActionSaveControlDataToFileClicked()
{

}

void MainWindow::onActionRestoreControlDataFromFileClicked()
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

void MainWindow::configureInboundDataTableView()
{
    // Add table model data and configure settings
    ui->tableViewStatusData->setModel(&(mainWindowController->getInboundDataTableModel()));
    ui->tableViewStatusData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    configureCommonTableSettings(ui->tableViewStatusData);
}

void MainWindow::configureOutboundDataTableView()
{
    // Add table model data and configure seettings
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
    table->horizontalHeader()->setFixedHeight(25);
    table->resizeColumnToContents(2);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionsClickable(false);

    // Configure the vertical header settings
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->hide();
}
