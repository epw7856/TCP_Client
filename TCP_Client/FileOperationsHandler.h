#ifndef FILEOPERATIONSHANDLER_H
#define FILEOPERATIONSHANDLER_H

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
    void initiateSaveOutboundDataToFile(const std::map<unsigned, DataItem*>& dataItems,
                                        QWidget* parent = nullptr);

    QString showFileSelectionDialog(const QString& title, QWidget* parent = nullptr);
    QString showFileSaveDialog(const QString& title, QWidget* parent = nullptr);

    static bool verifyFileSelection(const QString& filePath);
    static bool verifyFileExtension(const QString& filePath);
    static void showFileErrorPopup(const QString& title, const QString& message);
    static bool showConfigFileSelectionWarning();
    static void showFileOperationInProgressWarning();

signals:
    void requestWriteToFile(QJsonDocument document);

private:
    ApplicationInterface& appInterface;
    SettingsInterface& settingsInterface;
    QThread fileOpsThread;
    QJsonDocument fileContent;
    std::unique_ptr<FileTask> fileTask;
    bool fileOperationInProgress = false;

    void saveDefaultPath(const QString& filePath);
    QString getDefaultPath() const;
    void buildControlDataFileContent(const std::map<unsigned, DataItem*>& dataItems);
};

#endif // FILEOPERATIONSHANDLER_H
