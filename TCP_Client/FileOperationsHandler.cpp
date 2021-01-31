#include "FileOperationsHandler.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardPaths>

QString FileOperationsHandler::previousPath = "";

bool FileOperationsHandler::verifyFilePath(const QString& filePath)
{
    QFileInfo file(filePath);
    return ((file.exists()) && (file.suffix() == "json"));
}

void FileOperationsHandler::showConfigFileErrorPopup(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Configuration Error");
    msgBox.setText("<p align='center'>" + message + "</p>");
    msgBox.setFont(QFont("Segoe UI", 10));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

QString FileOperationsHandler::showFileSelectionDialog(QWidget* parent)
{
    QString defaultPath;
    (previousPath.isEmpty()) ? defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) :
                               defaultPath = previousPath;

    previousPath = QFileDialog::getOpenFileName(parent, "Select Configuration File", defaultPath, "JSON Files (*.json)");
    return previousPath;
}

bool FileOperationsHandler::showFileSelectionWarning()
{
    const QString message = "A connection with the server is active and data is currently being transmitted."
                            "Do you wish to proceed with the selection of a new configuration file?";

    QMessageBox msgBox;
    msgBox.setWindowTitle("Configuration Selection");
    msgBox.setText("<p align='center'>" + message + "</p>");
    msgBox.setFont(QFont("Segoe UI", 10));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Warning);
    int ret = msgBox.exec();
    return (ret == QMessageBox::Yes);
}
