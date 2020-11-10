#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QString>
#include <vector>

class DataInterface
{
public:
    DataInterface() {}
    virtual ~DataInterface() = default;

    // Enum Registry
    virtual std::vector<QString> getEnumStrings(QString enumName) const = 0;
    virtual QString getEnumStringValue(QString enumName, unsigned val) const = 0;
    virtual unsigned getEnumUintValue(QString enumName, QString enumString) const = 0;

    // Data From Server


};

#endif // DATAINTERFACE_H
