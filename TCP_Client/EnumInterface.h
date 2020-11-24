#ifndef ENUMINTERFACE_H
#define ENUMINTERFACE_H

#include <QString>
#include <vector>

class EnumInterface
{
public:
    EnumInterface() {}
    virtual ~EnumInterface() = default;

    // Enum registry API
    virtual std::vector<QString> getEnumStrings(const QString& enumName) const = 0;
    virtual QString getEnumStringValue(const QString& enumName, unsigned value) const = 0;
    virtual unsigned getEnumUintValue(const QString& enumName, const QString& enumString) const = 0;
};
#endif // ENUMINTERFACE_H
