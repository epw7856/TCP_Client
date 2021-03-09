#include "ApplicationInterface.h"
#include "DataItem.h"
#include "FileOperationsHandler.h"
#include "FileTask.h"
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QStandardPaths>
#include <QVariant>
#include "SettingsInterface.h"

Q_DECLARE_METATYPE(QJsonDocument);
Q_DECLARE_METATYPE(FileOperationStatus);

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
    qRegisterMetaType<FileOperationStatus>();

    connect(this, &FileOperationsHandler::requestWriteToFile, fileTask.get(), &FileTask::writeControlDataToFile);
    connect(fileTask.get(), &FileTask::writeOperationComplete, this, &FileOperationsHandler::receivedFileWriteCompleteNotification);

    connect(this, &FileOperationsHandler::requestRestoreFromFile, fileTask.get(), &FileTask::loadControlDataFromFile);
    connect(fileTask.get(), &FileTask::loadOperationComplete, this, &FileOperationsHandler::receivedFileLoadCompleteNotification);

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
    return (writeOperationInProgress || readOperationInProgress);
}

void FileOperationsHandler::initiateSaveControlDataToFile(const std::map<unsigned, DataItem*>& dataItems, QWidget* parent)
{
    if(isFileOperationInProgress())
    {
        showFileOperationInProgressWarning();
        return;
    }

    QString fileSaveName = showFileSaveDialog("Save Control Data", parent);
    if(!verifyFileExtension(fileSaveName))
    {
        showFileErrorPopup("File Error", "Invalid file name specified.");
        return;
    }

    writeOperationInProgress = true;
    QJsonDocument fileContent = buildJsonForSave(dataItems);
    emit requestWriteToFile(fileSaveName, fileContent);
}

void FileOperationsHandler::initiateRestoreControlDataFromFile(const std::map<unsigned, DataItem*>& dataItems, QWidget* parent)
{
    restoreDataItems.clear();
    restoreDataItems = dataItems;

    if(isFileOperationInProgress())
    {
        showFileOperationInProgressWarning();
        return;
    }

    QString fileRestoreName = showFileSelectionDialog("Restore Control Data", parent);
    if(!verifyFileSelection(fileRestoreName))
    {
        showFileErrorPopup("File Error", "Invalid file name specified.");
        return;
    }

    readOperationInProgress = true;
    emit requestRestoreFromFile(fileRestoreName);
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

QJsonDocument FileOperationsHandler::buildJsonForSave(const std::map<unsigned, DataItem*>& dataItems)
{
    QJsonObject lcaObj;
    QJsonObject obj;

    obj.insert("Configuration Version", appInterface.getConfigurationVersion());
    obj.insert("Date Saved", QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss"));

    QJsonArray dataItemArray;
    for(const auto& i : dataItems)
    {
        QJsonObject dataItemObj;
        dataItemObj.insert("Index", QString::number(i.first));
        dataItemObj.insert("Data Item Name", i.second->getDataItemName());
        dataItemObj.insert("Data Type", i.second->getDataItemType());
        dataItemObj.insert("Value", i.second->getDisplayValue());

        dataItemArray.push_back(dataItemObj);
    }

    obj.insert("Saved Control Data", QJsonValue(dataItemArray));
    lcaObj.insert("Local Control Application", QJsonValue(obj));
    QJsonDocument fileContent;
    fileContent.setObject(lcaObj);
    return fileContent;
}

void FileOperationsHandler::receivedFileWriteCompleteNotification(FileOperationStatus status)
{
    writeOperationInProgress = false;
    showFileOperationStatusMsg("Save Control Data", status);
}

void FileOperationsHandler::receivedFileLoadCompleteNotification(FileOperationStatus status, QJsonDocument contents)
{
    readOperationInProgress = false;

    const QString dialogTitle = "Restore Control Data";

    if(!status.success)
    {
        showFileOperationStatusMsg(dialogTitle, status);
        return;
    }

    QJsonObject lcaObj = contents.object().value("Local Control Application").toObject();

    if(!lcaObj.contains("Configuration Version") || !lcaObj.contains("Saved Control Data"))
    {
        status.success = false;
        status.message = "Unable to restore control data due to invalid or corrupted file content format.";

        showFileOperationStatusMsg(dialogTitle, status);
        return;
    }

    if(appInterface.getConfigurationVersion() != lcaObj.value("Configuration Version").toString())
    {
        status.success = false;
        status.message = "Unable to restore control data due to mismatched configuration version information.<br><br>"
                         "Current configuration version: " + appInterface.getConfigurationVersion() + "<br>"
                         "Detected configuration version from file: " + lcaObj.value("Configuration Version").toString();

        showFileOperationStatusMsg(dialogTitle, status);
        return;
    }

    std::vector<QString> displayValues;
    auto itr = restoreDataItems.begin();

    for (const QJsonValue& item : lcaObj.value("Saved Control Data").toArray())
    {
        if((item.toObject().value("Index").toVariant().toUInt() != (*itr).first) ||
           (item.toObject().value("Data Item Name").toString() != (*itr).second->getDataItemName()) ||
           (item.toObject().value("Data Type").toString() != (*itr).second->getDataItemType()))
        {
            status.success = false;
            break;
        }

        displayValues.push_back(item.toObject().value("Value").toString());
        itr++;
    }

    if(status.success)
    {
        status.message = "Successfully restored data items.";
        emit transmitRestoredDisplayValues(displayValues);
    }
    else
    {
        status.message = "Unable to restore control data due to mismatched data item information.";
    }

    showFileOperationStatusMsg(dialogTitle, status);
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
    const QString message = "A connection with the server is active and data is currently being transmitted. "
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

void FileOperationsHandler::showFileOperationStatusMsg(const QString& title, const FileOperationStatus& status)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText("<p align='center'>" + status.message + "</p>");
    msgBox.setFont(QFont("Segoe UI", 10));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    (status.success) ? msgBox.setIcon(QMessageBox::Information) :
                       msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}
