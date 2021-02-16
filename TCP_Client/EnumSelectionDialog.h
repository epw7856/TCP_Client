#ifndef ENUMSELECTIONDIALOG_H
#define ENUMSELECTIONDIALOG_H

#include <memory>
#include <QDialog>

class EnumSelectionController;
class EnumSelectionTableModel;

namespace Ui {
class EnumSelectionDialog;
}

class EnumSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnumSelectionDialog(QWidget *parent = nullptr);
    ~EnumSelectionDialog();

    void setDataItemName(const QString& value);
    void setEnumName(const QString& value);
    void setEnumStrings(const std::vector<QString>& enumStrings);
    void setCurrentValue(const QString& value);
    bool isAccepted() const;
    QString getNewValue() const;

private slots:
    void enumClicked(const QModelIndex& index);
    void enumDoubleClicked(const QModelIndex& index);
    void onButtonAcceptClicked();
    void onButtonCancelClicked();
    void showEvent(QShowEvent* event) override;

private:
    Ui::EnumSelectionDialog *ui;
    std::unique_ptr<EnumSelectionController> controller;
    std::unique_ptr<EnumSelectionTableModel> tableModel;
};

#endif // ENUMSELECTIONDIALOG_H
