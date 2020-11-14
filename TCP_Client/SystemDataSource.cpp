#include "DataItem.h"
#include "DataUtilities.h"
#include "SystemDataSource.h"

void SystemDataSource::initializeSystemConfig(const QString& configFilePath)
{

}

void SystemDataSource::updateDataItemDisplayValue(DataItem& dataItem)
{
    const QString& type = dataItem.getDataItemType();
    unsigned rawValue = dataItem.getRawValue();

    if(rawValue == UINT_MAX)
    {
        dataItem.setDisplayValue("ERROR");
        return;
    }

    if(type == "unsigned")
    {
        dataItem.setDisplayValue(QString::number(rawValue));
    }
    else if(type == "integer")
    {
        dataItem.setDisplayValue(QString::number(unsignedToInt(rawValue)));
    }
    else if(type == "float")
    {
        dataItem.setDisplayValue(QString().asprintf(dataItem.getDataItemFormat().toStdString().c_str(),
                                                    unsignedToFloat(rawValue)));
    }
    else
    {
        // Data Type is an enum
        dataItem.setDisplayValue(getEnumStringValue(type, rawValue));
    }
}

void SystemDataSource::updateDataItemRawValue(DataItem& dataItem)
{
    const QString& type = dataItem.getDataItemType();
    const QString& displayValue = dataItem.getDisplayValue();
    bool status = false;

    if(type == "unsigned")
    {
        unsigned intermediate = displayValue.toUInt(&status);
        if(status)
        {
            dataItem.setRawValue(intermediate);
        }
    }
    else if(type == "integer")
    {
        int intermediate = displayValue.toInt(&status);
        if(status)
        {
            dataItem.setRawValue(intToUnsigned(intermediate));
        }
    }
    else if(type == "float")
    {
        float intermediate = displayValue.toFloat(&status);
        if(status)
        {
            dataItem.setRawValue(floatToUnsigned(intermediate));
        }
    }
    else
    {
        // Data Type is an enum
        dataItem.setRawValue(getEnumUintValue(type, displayValue));
    }

    if(!status)
    {
        dataItem.setRawValue(UINT_MAX);
    }
}

std::vector<QString> SystemDataSource::getEnumStrings(const QString& enumName) const
{
    if(enumRegistry.find(enumName.toStdString()) != enumRegistry.end())
    {
        return enumRegistry.at(enumName.toStdString())->getEnumStrings();
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
        return enumRegistry.at(enumName.toStdString())->getEnumString(value);
    }
    else
    {
        return "UNKNOWN";
    }
}

unsigned SystemDataSource::getEnumUintValue(const QString& enumName, const QString& enumString) const
{
    if(enumRegistry.find(enumName.toStdString()) != enumRegistry.end())
    {
        return enumRegistry.at(enumName.toStdString())->getEnumUintValue(enumString);
    }
    else
    {
        return 0U;
    }
}

void SystemDataSource::setInboundRawValues(const std::vector<unsigned>& rawValues)
{
    if(rawValues.size() == inboundDataItems.size())
    {
        for(unsigned i = 0; i < rawValues.size(); ++i)
        {
            inboundDataItems[i]->setRawValue(rawValues[i]);
            updateDataItemDisplayValue(*(inboundDataItems[i]));
        }
    }
}

std::vector<QString> SystemDataSource::getInboundDisplayValues() const
{
    std::vector<QString> displayValues = {};
    for(const auto& item : inboundDataItems)
    {
        displayValues.push_back(item->getDisplayValue());
    }

    return displayValues;
}

void SystemDataSource::setOutboundRawValues(const std::vector<unsigned>& rawValues)
{

}

void SystemDataSource::setOutboundRawValue(unsigned index, unsigned rawValue)
{

}

std::vector<QString> SystemDataSource::getOutboundDisplayValues() const
{

}
