#include "SystemDataSource.h"

std::vector<QString> SystemDataSource::getEnumStrings(const QString& enumName) const
{
    if(enumRegistry.find(enumName.toStdString()) != enumRegistry.end())
    {
        return enumRegistry.at(enumName.toStdString());
    }
    else
    {
        return {};
    }
}

QString SystemDataSource::getEnumStringValue(const QString& enumName, unsigned value) const
{
    if(enumRegistry.find(enumName.toStdString()) != enumRegistry.end())
    {
        return enumRegistry.at(enumName.toStdString()).at(value);
    }
    else
    {
        return QString();
    }
}

unsigned SystemDataSource::getEnumUintValue(const QString& enumName, const QString& enumString) const
{
    std::vector<QString> enumStrings = enumRegistry.at(enumName.toStdString());
    auto itr = std::find(enumStrings.begin(), enumStrings.end(), enumString);

    return (itr != enumStrings.end()) ? std::distance(enumStrings.begin(), itr) : 0U;
}
