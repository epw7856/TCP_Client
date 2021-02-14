#include "ApplicationSettingsController.h"
#include "ApplicationSettingsDialog.h"
#include <QShowEvent>
#include "ui_ApplicationSettingsDialog.h"

ApplicationSettingsDialog::ApplicationSettingsDialog
(
    ApplicationInterface& appInterface,
    SettingsInterface& settingsInterface,
    QWidget *parent
)
:
    QDialog(parent),
    ui(new Ui::ApplicationSettingsDialog),
    controller(std::make_unique<ApplicationSettingsController>(appInterface, settingsInterface))
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(ui->pushButtonOk, &QPushButton::clicked, this, &ApplicationSettingsDialog::onPushButtonOkClicked);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &ApplicationSettingsDialog::onPushButtonCancelClicked);
}

ApplicationSettingsDialog::~ApplicationSettingsDialog()
{
    delete ui;
}

void ApplicationSettingsDialog::onPushButtonOkClicked()
{

}

void ApplicationSettingsDialog::onPushButtonCancelClicked()
{
    close();
}

void ApplicationSettingsDialog::showEvent(QShowEvent* event)
{
    ui->labelConfigFile->clear();
    ui->lineEditSocketPort->clear();
    ui->lineEditTransmissionPeriodicity->clear();

    ui->labelConfigFile->setText(controller->getCurrentConfig());
    ui->lineEditSocketPort->setText(QString::number(controller->getSocketPort()));
    ui->lineEditTransmissionPeriodicity->setText(QString::number(controller->getTransmissionPeriodicity()));
    ui->checkBoxConnectionNotifications->setChecked(controller->isShowConnectionNotificationsEnabled());
    ui->checkBoxAutoConnect->setChecked(controller->isAutoConnectEnabled());

    event->accept();
}
