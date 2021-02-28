#ifndef FILETASK_H
#define FILETASK_H

#include "FileOperationStatus.h"
#include <QJsonDocument>
#include <QObject>

class FileTask : public QObject
{
    Q_OBJECT

public:
    FileTask() {}

public slots:
    void writeControlDataToFile(QString filePath, QJsonDocument contents);
    void loadControlDataFromFile(QString filePath);

signals:
    void writeOperationComplete(FileOperationStatus status);
    void loadOperationComplete(FileOperationStatus status, QJsonDocument contents);
};

#endif // FILETASK_H
