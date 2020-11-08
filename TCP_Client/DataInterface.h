#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <string>
#include <vector>

class DataInterface
{
public:
    DataInterface() {}
    virtual ~DataInterface() = default;

    // Enum Registry
    virtual std::vector<std::string> getEnumStrings(std::string enumName) const = 0;
    virtual std::string getEnumStringValue(std::string enumName, unsigned val) const = 0;
    virtual unsigned getEnumUintValue(std::string enumName, std::string enumString) const = 0;

    // Data From Server


};

#endif // DATAINTERFACE_H
