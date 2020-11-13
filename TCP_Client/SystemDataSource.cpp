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
