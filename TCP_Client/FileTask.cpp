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
       status.message = "Unable to save control data to output file. Please ensure that the specified "
                        "file is not locked for editing.";
    }

    if(status.success)
    {
        file.write(contents.toJson());
        status.message = "Successfully saved control data to output file.";
    }

    emit writeOperationComplete(status);
}

void FileTask::loadControlDataFromFile(QString filePath)
{
    FileOperationStatus status;
    status.success = true;

    QFile file(filePath);
    QJsonDocument doc;

    if(!file.open((QIODevice::ReadOnly | QIODevice::Text)))
    {
        status.success = false;
        status.message = "Unable to open file. Please ensure that the specified file exists and "
                         "is assigned the proper access permissions.";
    }

    if(status.success)
    {
        QByteArray rawData = file.readAll();
        file.close();
        doc = QJsonDocument::fromJson(rawData);
        status.message = "Successfully loaded control data from file.";
    }

    emit loadOperationComplete(status, doc);
}
