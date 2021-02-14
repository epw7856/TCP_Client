#ifndef APPLICATIONSETTINGSDIALOG_H
#define APPLICATIONSETTINGSDIALOG_H

#include <memory>
#include <QDialog>

class ApplicationInterface;
class ApplicationSettingsController;
class SettingsInterface;

namespace Ui {
class ApplicationSettingsDialog;
}

class ApplicationSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApplicationSettingsDialog(ApplicationInterface& appInterface,
                                       SettingsInterface& settingsInterface,
                                       QWidget *parent = nullptr);
    ~ApplicationSettingsDialog();

public slots:
    void onPushButtonOkClicked();
    void onPushButtonCancelClicked();

private slots:
    void showEvent(QShowEvent* event) override;

private:
    Ui::ApplicationSettingsDialog *ui;
    std::unique_ptr<ApplicationSettingsController> controller;
};

#endif // APPLICATIONSETTINGSDIALOG_H
