#include "MainWindow.h"
#include "MainWindowController.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(const QString& configFilePathArg, QWidget *parent)
:
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mainWindowController(std::make_unique<MainWindowController>(configFilePathArg)),
    inboundDataTableModel(mainWindowController->getInboundDataInterface())
{
    ui->setupUi(this);

    // Menu bar signal/slot connections
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);
    connect(ui->actionLoadSystemConfigurationFile, &QAction::triggered, this, &MainWindow::onActionLoadSystemConfigurationFileTriggered);
    connect(ui->actionViewApplicationConfiguration, &QAction::triggered, this, &MainWindow::onActionViewApplicationConfigurationTriggered);
    connect(ui->actionConnectToServer, &QAction::triggered, this, &MainWindow::onActionConnectToServerTriggered);
    connect(ui->actionDisconnectFromServer, &QAction::triggered, this, &MainWindow::onActionDisconnectFromServerTriggered);

    configureInboundDataTableView();
}

MainWindow::~MainWindow()
{
    delete ui;
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

}

void MainWindow::onActionDisconnectFromServerTriggered()
{

}

void MainWindow::configureInboundDataTableView()
{
    // Add table model data and disable selection
    ui->tableViewStatusData->setModel(&inboundDataTableModel);
    ui->tableViewStatusData->setSelectionMode(QAbstractItemView::NoSelection);

    // Set bold font for the header
    QFont font(ui->tableViewStatusData->font());
    font.setBold(true);
    ui->tableViewStatusData->horizontalHeader()->setFont(font);
    ui->tableViewStatusData->setStyleSheet("QHeaderView::section { background-color: rgb(240, 240, 240) }");

    // Disable cell resizing and selections
    ui->tableViewStatusData->horizontalHeader()->setFixedHeight(25);
    ui->tableViewStatusData->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableViewStatusData->verticalHeader()->hide();
    ui->tableViewStatusData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewStatusData->setFocusPolicy(Qt::NoFocus);
    ui->tableViewStatusData->setSelectionMode(QAbstractItemView::NoSelection);
}
