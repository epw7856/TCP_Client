#include "ApplicationInterface.h"
#include "DataItem.h"
#include "FileOperationsHandler.h"
#include "FileTask.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QStandardPaths>
#include "SettingsInterface.h"

Q_DECLARE_METATYPE(QJsonDocument);
Q_DECLARE_METATYPE(QString);

FileOperationsHandler::FileOperationsHandler
(
    ApplicationInterface& localAppInterface,
    SettingsInterface& localSettingsInterface
)
:
    appInterface(localAppInterface),
    settingsInterface(localSettingsInterface),
    fileTask(std::make_unique<FileTask>())
{
    qRegisterMetaType<QJsonDocument>();
    qRegisterMetaType<QString>();

    fileTask->moveToThread(&fileOpsThread);
    fileOpsThread.start();
}

FileOperationsHandler::~FileOperationsHandler()
{
    fileOpsThread.quit();
    fileOpsThread.wait(2000);
}

bool FileOperationsHandler::isFileOperationInProgress() const
{
    return fileOperationInProgress;
}

void FileOperationsHandler::initiateSaveOutboundDataToFile(const std::map<unsigned, DataItem*>& dataItems, QWidget* parent)
{
    if(isFileOperationInProgress())
    {
        showFileOperationInProgressWarning();
        return;
    }

    QString fileSaveName = showFileSaveDialog("Save Control Data", parent);
    if(!verifyFileExtension(fileSaveName))
    {
        showFileErrorPopup("File Save Error", "Invalid file name specified.");
        return;
    }

    fileOperationInProgress = true;
    buildControlDataFileContent(dataItems);
}

bool FileOperationsHandler::verifyFileSelection(const QString& filePath)
{
    QFileInfo file(filePath);
    return ((file.exists()) && (file.suffix() == "json"));
}

bool FileOperationsHandler::verifyFileExtension(const QString& filePath)
{
    QFileInfo file(filePath);
    return (!file.fileName().isEmpty() && file.suffix() == "json");
}

QString FileOperationsHandler::showFileSelectionDialog(const QString& title, QWidget* parent)
{
    const QString fileSelection = QFileDialog::getOpenFileName(parent,
                                                               title,
                                                               getDefaultPath(),
                                                               "JSON File (*.json)");
    saveDefaultPath(fileSelection);
    return fileSelection;
}

QString FileOperationsHandler::showFileSaveDialog(const QString& title, QWidget* parent)
{
    const QString fileSelection = QFileDialog::getSaveFileName(parent,
                                                               title,
                                                               getDefaultPath(),
                                                               "JSON File (*.json)");
    saveDefaultPath(fileSelection);
    return fileSelection;
}

void FileOperationsHandler::saveDefaultPath(const QString& filePath)
{
    QFileInfo file(filePath);
    settingsInterface.setFileSelectionPath(file.path());
}

QString FileOperationsHandler::getDefaultPath() const
{
    QString defaultPath = settingsInterface.getFileSelectionPath();

    QDir dir(defaultPath);
    return (dir.exists() ? defaultPath :
                           QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
}

void FileOperationsHandler::buildControlDataFileContent(const std::map<unsigned, DataItem*>& dataItems)
{
    QJsonObject lcaObj;
    QJsonObject obj;

    obj.insert("File Type", "Saved Control Data");
    obj.insert("Configuration Version", appInterface.getConfigurationVersion());

    QJsonArray dataItemArray;
    for(const auto& i : dataItems)
    {
        QJsonObject dataItemObj;
        dataItemObj.insert("Index", QString::number(i.first));
        dataItemObj.insert("Data Item Name", i.second->getDataItemName());
        dataItemObj.insert("Data Item Type", i.second->getDataItemType());
        dataItemObj.insert("Value", i.second->getDisplayValue());

        dataItemArray.push_back(dataItemObj);
    }
    obj.insert("Control Data Items", QJsonValue(dataItemArray));
    lcaObj.insert("Local Control Application", QJsonValue(obj));
    fileContent.setObject(lcaObj);
}

void FileOperationsHandler::showFileErrorPopup(const QString& title, const QString& message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText("<p align='center'>" + message + "</p>");
    msgBox.setFont(QFont("Segoe UI", 10));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

bool FileOperationsHandler::showConfigFileSelectionWarning()
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

void FileOperationsHandler::showFileOperationInProgressWarning()
{
    const QString msg = "File operation in progress. Please wait until the operation completes"
                        "prior to initiating another.";
    QMessageBox msgBox;
    msgBox.setWindowTitle("File Operation");
    msgBox.setText("<p align='center'>" + msg + "</p>");
    msgBox.setFont(QFont("Segoe UI", 10));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}
