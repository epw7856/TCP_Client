#include "MainWindow.h"
#include "MainWindowController.h"
#include <QLabel>
#include "ui_MainWindow.h"

MainWindow::MainWindow(const QString& configFilePathArg, QWidget *parent)
:
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mainWindowController(std::make_unique<MainWindowController>(configFilePathArg)),
    inboundDataTableModel(mainWindowController->getInboundDataInterface())
{
    ui->setupUi(this);

    // Connections with the menu bar in the UI
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);
    connect(ui->actionLoadSystemConfigurationFile, &QAction::triggered, this, &MainWindow::onActionLoadSystemConfigurationFileTriggered);
    connect(ui->actionViewApplicationConfiguration, &QAction::triggered, this, &MainWindow::onActionViewApplicationConfigurationTriggered);
    connect(ui->actionConnectToServer, &QAction::triggered, this, &MainWindow::onActionConnectToServerTriggered);
    connect(ui->actionDisconnectFromServer, &QAction::triggered, this, &MainWindow::onActionDisconnectFromServerTriggered);

    // Connections from MainWindowController to MainWindow
    connect(mainWindowController.get(), &MainWindowController::sendStatusBarMessage, this, &MainWindow::showStatusBarMessage);

    configureInboundDataTableView();
    setupStatusBar();
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
    showStatusBarMessage("Ready");
}

void MainWindow::showStatusBarMessage(QString msg)
{
    statusBarLabel->clear();
    statusBarLabel->setText("Status: " + msg + "  ");
}

void MainWindow::refreshInboundDataDisplay()
{
    inboundDataTableModel.layoutChanged();
    //ui->tableViewStatusData->update();
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
    showStatusBarMessage("Connecting...");
}

void MainWindow::onActionDisconnectFromServerTriggered()
{

}

void MainWindow::configureInboundDataTableView()
{
    // Add table model data and disable selection
    ui->tableViewStatusData->setModel(&inboundDataTableModel);
    ui->tableViewStatusData->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewStatusData->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewStatusData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewStatusData->setFocusPolicy(Qt::NoFocus);

    // Set bold font for the header
    QFont font(ui->tableViewStatusData->font());
    font.setBold(true);
    ui->tableViewStatusData->horizontalHeader()->setFont(font);
    ui->tableViewStatusData->setStyleSheet("QHeaderView::section { background-color: rgb(240, 240, 240) }");

    ui->tableViewStatusData->horizontalHeader()->setFixedHeight(25);
    ui->tableViewStatusData->resizeColumnToContents(2);
    ui->tableViewStatusData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableViewStatusData->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableViewStatusData->verticalHeader()->hide();
}
