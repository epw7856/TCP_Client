#include "MainWindow.h"
#include "MainWindowController.h"
#include <QLabel>
#include "ui_MainWindow.h"

MainWindow::MainWindow(const QString& configFilePathArg, QWidget *parent)
:
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mainWindowController(std::make_unique<MainWindowController>(configFilePathArg)),
    inboundDataTableModel(mainWindowController->getInboundDataInterface()),
    outboundDataTableModel(mainWindowController->getOutboundDataInterface())
{
    ui->setupUi(this);

    // Connections with the menu bar in the UI
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onActionExitTriggered);
    connect(ui->actionLoadSystemConfigurationFile, &QAction::triggered, this, &MainWindow::onActionLoadSystemConfigurationFileTriggered);
    connect(ui->actionViewApplicationConfiguration, &QAction::triggered, this, &MainWindow::onActionViewApplicationConfigurationTriggered);
    connect(ui->actionConnectToServer, &QAction::triggered, this, &MainWindow::onActionConnectToServerTriggered);
    connect(ui->actionDisconnectFromServer, &QAction::triggered, this, &MainWindow::onActionDisconnectFromServerTriggered);

    // Connections with the pushbuttons in the UI
    connect(ui->pushButtonSaveToFile, &QPushButton::clicked, this, &MainWindow::onButtonSaveToFileClicked);
    connect(ui->pushButtonRestoreFromFile, &QPushButton::clicked, this, &MainWindow::onButtonRestoreFromFileClicked);

    // Connections from MainWindowController to MainWindow
    connect(mainWindowController.get(), &MainWindowController::sendStatusBarMessage, this, &MainWindow::showStatusBarMessage);
    connect(mainWindowController.get(), &MainWindowController::notifyInboundDataUpdated, this, &MainWindow::refreshStatusDataDisplay);
    connect(mainWindowController.get(), &MainWindowController::notifyStatusChange, this, &MainWindow::periodicUpdate);

    // Setup UI components
    configureInboundDataTableView();
    configureOutboundDataTableView();
    setupStatusBar();

    // Perform initial setup and refresh the UI
    mainWindowController->performInitialSetup();
    periodicUpdate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupStatusBar()
{
    statusBarLabel = std::make_unique<QLabel>();
    statusBarLabel->setFont(QFont("Segoe UI", 10));
    ui->statusBar->setStyleSheet("QStatusBar{border-top: 1px outset grey;}");
    ui->statusBar->addPermanentWidget(statusBarLabel.get());
    ui->statusBar->setSizeGripEnabled(false);
}

void MainWindow::showStatusBarMessage(QString msg)
{
    statusBarLabel->clear();
    statusBarLabel->setText("Status: " + msg + "  ");
}

void MainWindow::refreshStatusDataDisplay()
{
    inboundDataTableModel.layoutChanged();
    ui->tableViewStatusData->update();
}

void MainWindow::periodicUpdate()
{
    ui->actionConnectToServer->setEnabled(mainWindowController->enableActionConnectToServer());
    ui->actionDisconnectFromServer->setEnabled(mainWindowController->enableActionDisconnectFromServer());
    ui->pushButtonSaveToFile->setEnabled(mainWindowController->enableButtonSaveToFile());
    ui->pushButtonRestoreFromFile->setEnabled(mainWindowController->enableButtonRestoreFromFile());
}

void MainWindow::onActionAboutTriggered()
{

}

void MainWindow::onActionExitTriggered()
{
    close();
}

void MainWindow::onActionLoadSystemConfigurationFileTriggered()
{

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

void MainWindow::onButtonSaveToFileClicked()
{

}

void MainWindow::onButtonRestoreFromFileClicked()
{

}

void MainWindow::configureInboundDataTableView()
{
    // Add table model data
    ui->tableViewStatusData->setModel(&inboundDataTableModel);
    ui->tableViewStatusData->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewStatusData->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewStatusData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewStatusData->setFocusPolicy(Qt::NoFocus);

    // Set bold font for the header
    QFont font(ui->tableViewStatusData->font());
    font.setBold(true);
    ui->tableViewStatusData->horizontalHeader()->setFont(font);
    ui->tableViewStatusData->setStyleSheet("QHeaderView::section { background-color: rgb(240, 240, 240) }\n"
                                           "QTableView::item:selected {background-color: rgb(68, 93, 249);}");

    ui->tableViewStatusData->horizontalHeader()->setFixedHeight(25);
    ui->tableViewStatusData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableViewStatusData->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableViewStatusData->verticalHeader()->hide();
}

void MainWindow::configureOutboundDataTableView()
{
    // Add table model data
    ui->tableViewControlData->setModel(&outboundDataTableModel);
    ui->tableViewControlData->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewControlData->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewControlData->setFocusPolicy(Qt::NoFocus);

    // Set bold font for the header
    QFont font(ui->tableViewControlData->font());
    font.setBold(true);
    ui->tableViewControlData->horizontalHeader()->setFont(font);
    ui->tableViewControlData->setStyleSheet("QHeaderView::section { background-color: rgb(240, 240, 240) }\n"
                                            "QTableView::item:selected {background-color: rgb(68, 93, 249);}");

    ui->tableViewControlData->horizontalHeader()->setFixedHeight(25);
    ui->tableViewControlData->resizeColumnToContents(2);
    ui->tableViewControlData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableViewControlData->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableViewControlData->verticalHeader()->hide();
}
