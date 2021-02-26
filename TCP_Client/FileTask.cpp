#include "FileTask.h"
#include <QFile>

void FileTask::writeControlDataToFile(QString filePath, QJsonDocument contents)
{
    FileOperationStatus status;
    status.success = true;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
       status.success = false;
       status.message = "Unable to save control data to output file. Please ensure that the specified file is not locked for editing.";
    }

    if(status.success)
    {
        file.write(contents.toJson());
        status.message = "Successfully saved control data to ouput file.";
    }

    emit writeOperationComplete(status);
}
