#include "EnumSelectionController.h"
#include "EnumSelectionDialog.h"
#include "ui_EnumSelectionDialog.h"

EnumSelectionDialog::EnumSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnumSelectionDialog),
    controller(std::make_unique<EnumSelectionController>())
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(ui->pushButtonAccept, &QPushButton::clicked, this, &EnumSelectionDialog::onButtonAcceptClicked);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &EnumSelectionDialog::onButtonCancelClicked);
}

EnumSelectionDialog::~EnumSelectionDialog()
{
    delete ui;
}

void EnumSelectionDialog::onButtonAcceptClicked()
{

}

void EnumSelectionDialog::onButtonCancelClicked()
{
    close();
}
