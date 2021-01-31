#ifndef FILEOPERATIONSHANDLER_H
#define FILEOPERATIONSHANDLER_H

#include <QWidget>

class FileOperationsHandler
{
public:
    static bool verifyFilePath(const QString& filePath);
    static void showConfigFileErrorPopup(const QString& message);
    static QString showFileSelectionDialog(QWidget* parent = nullptr);
    static bool showFileSelectionWarning();

private:
    static QString previousPath;
};

#endif // FILEOPERATIONSHANDLER_H
