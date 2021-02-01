#include "AboutDialog.h"
#include "SemanticVersion.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog),
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
    ui->labelVersion->setText(SemanticVersion);
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
