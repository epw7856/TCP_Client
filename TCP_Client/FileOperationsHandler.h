#ifndef FILEOPERATIONSHANDLER_H
#define FILEOPERATIONSHANDLER_H

#include <QJsonDocument>

class ApplicationInterface;
class DataItem;
class SettingsInterface;

class FileOperationsHandler
{
public:
    FileOperationsHandler(ApplicationInterface& localAppInterface, SettingsInterface& localSettingsInterface);

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

private:
    ApplicationInterface& appInterface;
    SettingsInterface& settingsInterface;
    bool fileOperationInProgress = false;
    QJsonDocument fileContent;

    void saveDefaultPath(const QString& filePath);
    QString getDefaultPath() const;
    void buildControlDataFileContent(const std::map<unsigned, DataItem*>& dataItems);
};

#endif // FILEOPERATIONSHANDLER_H
