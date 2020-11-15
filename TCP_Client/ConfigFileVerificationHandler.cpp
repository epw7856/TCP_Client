#include "ConfigFileVerificationHandler.h"
#include <QFileInfo>
#include <QMessageBox>

bool ConfigFileVerificationHandler::verifyFilePath(const QString& filePath)
{
    QFileInfo file(filePath);
    if(!file.exists() || (file.suffix() != "json"))
    {
        const QString msg = QString("%1").arg("The configuration file '" + filePath + "' is invalid or does not exist.");
        showConfigFileErrorPopup(msg);
        return false;
    }
    else
    {
        return true;
    }
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
