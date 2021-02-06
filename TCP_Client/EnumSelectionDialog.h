#ifndef ENUMSELECTIONDIALOG_H
#define ENUMSELECTIONDIALOG_H

#include <memory>
#include <QDialog>

class EnumSelectionController;

namespace Ui {
class EnumSelectionDialog;
}

class EnumSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnumSelectionDialog(QWidget *parent = nullptr);
    ~EnumSelectionDialog();

private slots:
    void onButtonAcceptClicked();
    void onButtonCancelClicked();

private:
    Ui::EnumSelectionDialog *ui;
    std::unique_ptr<EnumSelectionController> controller;
};

#endif // ENUMSELECTIONDIALOG_H
