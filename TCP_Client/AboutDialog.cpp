#include "AboutDialog.h"
#include "ApplicationInterface.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(const ApplicationInterface& localAppInterface, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog),
    appInterface(localAppInterface),
    scene(std::make_unique<QGraphicsScene>())
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QPixmap map(":/icons/logo.jpg");
    scene->addPixmap(map);
    ui->graphicsViewLogo->setScene(scene.get());
    ui->graphicsViewLogo->setMinimumWidth(map.width());
    ui->graphicsViewLogo->setMinimumHeight(map.height());
    ui->graphicsViewLogo->show();
    setFixedSize(width(), height());

    connect(ui->pushButtonClose, &QPushButton::clicked, this, &AboutDialog::onActionClose);

    ui->labelName->setText("Local Control Application");
    ui->labelVersion->setText(appInterface.getSemanticVersion());
    ui->labelDepartment->setText("Shipboard Electrical Systems");
    ui->labelOrganization->setText("Naval Nuclear Laboratory");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::onActionClose()
{
    close();
}
