#include "EnumType.h"

std::vector<QString> EnumType::getEnumStrings() const
{
    std::vector<QString> strings = {};
    for(const auto& entry : entries)
    {
        strings.push_back(entry.name);
    }

    return strings;
}

QString EnumType::getEnumString(unsigned value) const
{
    for(const auto& entry : entries)
    {
        if(entry.value == value)
        {
            return entry.name;
        }
    }

    return "UNKNOWN";
}

unsigned EnumType::getEnumUintValue(const QString& enumString) const
{
    for(const auto& entry : entries)
    {
        if(entry.name == enumString)
        {
            return entry.value;
        }
    }

    return 0U;
}

void EnumType::addEnumEntry(const QString& enumString)
{
    EnumEntry entry;
    entry.name = std::move(enumString);
    entry.value = nextValue++;
    entries.push_back(entry);
}

void EnumType::addEnumEntry(const QString& enumString, unsigned value)
{
    EnumEntry entry;
    entry.name = std::move(enumString);
    entry.value = value;
    nextValue = ++value;
    entries.push_back(entry);
}
