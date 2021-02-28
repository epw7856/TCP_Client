#ifndef FILEOPERATIONSHANDLER_H
#define FILEOPERATIONSHANDLER_H

#include "FileOperationStatus.h"
#include <memory>
#include <QJsonDocument>
#include <QThread>

class ApplicationInterface;
class DataItem;
class FileTask;
class SettingsInterface;

class FileOperationsHandler : public QObject
{
    Q_OBJECT

public:
    FileOperationsHandler(ApplicationInterface& localAppInterface,
                          SettingsInterface& localSettingsInterface);

    ~FileOperationsHandler();

    bool isFileOperationInProgress() const;
    void initiateSaveControlDataToFile(const std::map<unsigned, DataItem*>& dataItems,
                                       QWidget* parent = nullptr);
    void initiateRestoreControlDataFromFile(const std::map<unsigned, DataItem*>& dataItems,
                                            QWidget* parent = nullptr);

    QString showFileSelectionDialog(const QString& title, QWidget* parent = nullptr);
    QString showFileSaveDialog(const QString& title, QWidget* parent = nullptr);

    static bool verifyFileSelection(const QString& filePath);
    static bool verifyFileExtension(const QString& filePath);
    static void showFileErrorPopup(const QString& title, const QString& message);
    static bool showConfigFileSelectionWarning();
    static void showFileOperationInProgressWarning();
    static void showFileOperationStatusMsg(const QString& title, const FileOperationStatus& status);

public slots:
    void receivedFileWriteCompleteNotification(FileOperationStatus status);
    void receivedFileLoadCompleteNotification(FileOperationStatus status, QJsonDocument contents);

signals:
    void requestWriteToFile(QString filePath, QJsonDocument document);
    void requestRestoreFromFile(QString filePath);
    void transmitRestoredDisplayValues(std::vector<QString> values);

private:
    ApplicationInterface& appInterface;
    SettingsInterface& settingsInterface;
    QThread fileOpsThread;
    std::unique_ptr<FileTask> fileTask;
    bool writeOperationInProgress = false;
    bool readOperationInProgress = false;
    std::map<unsigned, DataItem*> restoreDataItems;

    void saveDefaultPath(const QString& filePath);
    QString getDefaultPath() const;
    QJsonDocument buildJsonForSave(const std::map<unsigned, DataItem*>& dataItems);
;};

#endif // FILEOPERATIONSHANDLER_H
