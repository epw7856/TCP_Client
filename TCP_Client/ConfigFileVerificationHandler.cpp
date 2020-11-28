#include "ConfigFileVerificationHandler.h"
#include <QFileInfo>
#include <QMessageBox>

bool ConfigFileVerificationHandler::verifyFilePath(const QString& filePath)
{
    QFileInfo file(filePath);
    return ((file.exists()) && (file.suffix() == "json"));
}

void ConfigFileVerificationHandler::showConfigFileErrorPopup(const QString& message)
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
