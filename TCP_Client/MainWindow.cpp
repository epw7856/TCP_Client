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
}

MainWindow::~MainWindow()
{
    delete ui;
}

