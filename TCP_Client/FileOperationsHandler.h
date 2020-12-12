#ifndef FILEOPERATIONSHANDLER_H
#define FILEOPERATIONSHANDLER_H

#include <QString>

class FileOperationsHandler
{
public:
    static bool verifyFilePath(const QString& filePath);
    static void showConfigFileErrorPopup(const QString& message);
};

#endif // FILEOPERATIONSHANDLER_H
