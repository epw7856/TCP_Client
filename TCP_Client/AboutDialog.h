#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <memory>
#include <QDialog>

class ApplicationInterface;
class QGraphicsScene;

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(const ApplicationInterface& localAppInterface, QWidget *parent = nullptr);
    ~AboutDialog();

private slots:
    void onActionClose();

private:
    Ui::AboutDialog *ui;
    const ApplicationInterface& appInterface;
    std::unique_ptr<QGraphicsScene> scene;
};

#endif // ABOUTDIALOG_H
