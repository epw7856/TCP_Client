#ifndef FILEOPERATIONSTATUS_H
#define FILEOPERATIONSTATUS_H

#include <QString>

struct FileOperationStatus
{
    bool success = false;
    QString message = QString();
};

#endif // FILEOPERATIONSTATUS_H
