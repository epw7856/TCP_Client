#include "EnumSelectionController.h"
#include "EnumSelectionDialog.h"
#include "EnumSelectionTableModel.h"
#include <QShowEvent>
#include "ui_EnumSelectionDialog.h"

EnumSelectionDialog::EnumSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnumSelectionDialog),
    controller(std::make_unique<EnumSelectionController>()),
    tableModel(std::make_unique<EnumSelectionTableModel>())
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->listViewEnumValues->setModel(tableModel.get());
    ui->pushButtonAccept->setEnabled(false);

    connect(ui->listViewEnumValues, &QListView::clicked, this, &EnumSelectionDialog::enumClicked);
    connect(ui->listViewEnumValues, &QListView::doubleClicked, this, &EnumSelectionDialog::enumClicked);
    connect(ui->pushButtonAccept, &QPushButton::clicked, this, &EnumSelectionDialog::onButtonAcceptClicked);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &EnumSelectionDialog::onButtonCancelClicked);
}

EnumSelectionDialog::~EnumSelectionDialog()
{
    delete ui;
}

void EnumSelectionDialog::setDataItemName(const QString& value)
{
    ui->labelDataItemName->setText(value);
}

void EnumSelectionDialog::setEnumName(const QString& value)
{
    ui->labelEnumName->setText(value);
}

void EnumSelectionDialog::setEnumStrings(const std::vector<QString>& enumStrings)
{
    tableModel->setEnumStrings(enumStrings);
}

void EnumSelectionDialog::setCurrentValue(const QString& value)
{
    ui->labelCurrentValue->setText(value);
}

bool EnumSelectionDialog::isAccepted() const
{
    return controller->isAccepted();
}

QString EnumSelectionDialog::getNewValue() const
{
    return controller->getNewValue();
}

void EnumSelectionDialog::enumClicked(const QModelIndex& index)
{
    if(index.row() < tableModel->rowCount())
    {
        controller->setNewValue(tableModel->data(index).toString(), static_cast<unsigned>(index.row()));
        ui->labelNewValue->setText(controller->getNewValue());
        ui->pushButtonAccept->setEnabled(controller->enableAcceptButton());
    }
}

void EnumSelectionDialog::enumDoubleClicked(const QModelIndex& index)
{
    enumClicked(index);
    accept();
}

void EnumSelectionDialog::onButtonAcceptClicked()
{
    QModelIndex currentIndex = ui->listViewEnumValues->selectionModel()->currentIndex();
    if(controller->accept(tableModel->data(currentIndex).toString(), static_cast<unsigned>(currentIndex.row())))
    {
        close();
    }
}

void EnumSelectionDialog::onButtonCancelClicked()
{
    close();
}

void EnumSelectionDialog::showEvent(QShowEvent* event)
{
    ui->labelNewValue->clear();
    event->accept();
}
