#include "MainWindow.h"
#include "MainWindowController.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(const QString& configFilePathArg, QWidget *parent)
:
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mainWindowController(std::make_unique<MainWindowController>(configFilePathArg))
{
    ui->setupUi(this);

    // Menu bar signal/slot connections
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);
    connect(ui->actionLoadSystemConfigurationFile, &QAction::triggered, this, &MainWindow::onActionLoadSystemConfigurationFileTriggered);
    connect(ui->actionViewApplicationConfiguration, &QAction::triggered, this, &MainWindow::onActionViewApplicationConfigurationTriggered);
    connect(ui->actionConnectToServer, &QAction::triggered, this, &MainWindow::onActionConnectToServerTriggered);
    connect(ui->actionDisconnectFromServer, &QAction::triggered, this, &MainWindow::onActionDisconnectFromServerTriggered);
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
