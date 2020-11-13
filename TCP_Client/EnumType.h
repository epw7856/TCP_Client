#ifndef ENUMTYPE_H
#define ENUMTYPE_H

#include <QString>
#include <vector>

class EnumType
{
public:
    EnumType(const QString& enumSetName): setName(std::move(enumSetName)) {}

    QString getEnumSetName() const;
    std::vector<QString> getEnumStrings() const;
    QString getEnumString(unsigned value) const;
    unsigned getEnumUintValue(const QString& enumString) const;
    void addEnumEntry(const QString& enumString);
    void addEnumEntry(const QString& enumString, unsigned value);

private:
    struct EnumEntry
    {
        QString name = "";
        unsigned value = 0U;
    };

    std::vector<EnumEntry> entries = {};
    QString setName = "";
    unsigned nextValue = 0U;
};

inline QString EnumType::getEnumSetName() const
{
    return setName;
}

#endif // ENUMTYPE_H
