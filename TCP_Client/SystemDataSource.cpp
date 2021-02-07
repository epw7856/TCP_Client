#include "DataItem.h"
#include "DataUtilities.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include "SystemDataSource.h"

bool SystemDataSource::loadSystemConfiguration(const QString& configFilePath)
{
    appSettings.systemConfigFilePath = QString();
    systemConfigFile.setFileName(configFilePath);

    if(!systemConfigFile.open((QIODevice::ReadOnly | QIODevice::Text)))
    {
        return false;
    }

    appSettings.systemConfigFilePath = configFilePath;

    QByteArray rawData = systemConfigFile.readAll();
    systemConfigFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    obj = doc.object().value("Local Control Application").toObject();

    // Clear system configuration prior to loading new data
    clearSystemData();

    parseApplicationSettings();
    parseEnumerations();
    parseInboundData();
    parseOutboundData();

    // Data table ranges must be parsed after the inbound and outbound data items
    parseInboundDataTableRanges();
    parseOutboundDataTableRanges();

    return true;
}

void SystemDataSource::clearSystemData()
{
    enumRegistry.clear();
    inboundDataItems.clear();
    outboundDataItems.clear();
    inboundDataTableRanges.clear();
    outboundDataTableRanges.clear();
}

void SystemDataSource::parseApplicationSettings()
{
    const QJsonValue jsonAppSettings = obj.value("Application Settings");
    appSettings.socketPort = jsonStringToUnsigned(jsonAppSettings.toObject().value("Socket Port").toVariant().toString());
    appSettings.transmissionPeriodicity = jsonStringToUnsigned(jsonAppSettings.toObject().value("Transmission Periodicity (ms)").toVariant().toString());
}

void SystemDataSource::parseEnumerations()
{
    const QJsonValue jsonEnums = obj.value("Enumerations");
    for (const QJsonValue& item : jsonEnums.toArray())
    {
        const QString setName = QString(item.toObject().value("Set Name").toString());
        std::shared_ptr<EnumType> enumSet = std::make_shared<EnumType>(setName);

        const QJsonArray enumNames = item.toObject().value("Enum Names").toArray();
        for(const QJsonValue& name : enumNames)
        {
            if(name.toObject().contains("Value"))
            {
                unsigned value = jsonStringToUnsigned(name.toObject().value("Value").toVariant().toString());
                enumSet->addEnumEntry(name.toObject().value("Name").toString(), value);
            }
            else
            {
                enumSet->addEnumEntry(name.toObject().value("Name").toString());
            }
        }

        enumRegistry.insert({setName.toStdString(), enumSet});
    }
}

void SystemDataSource::parseInboundData()
{
    const QJsonValue dataItems = obj.value("Data From Server").toObject().value("Data Items");
    for (const QJsonValue& item : dataItems.toArray())
    {
        std::shared_ptr<DataItem> dataItem = std::make_shared<DataItem>(QString(item.toObject().value("Data Type").toString()),
                                                                        QString(item.toObject().value("Name").toString()),
                                                                        QString(item.toObject().value("Units").toString()),
                                                                        QString(item.toObject().value("Format").toString()));
        inboundDataItems.push_back(dataItem);
    }
}

void SystemDataSource::parseInboundDataTableRanges()
{
    const QJsonObject dataFromServerObj = obj.value("Data From Server").toObject();

    if(dataFromServerObj.contains("Status Data Table Displayed Ranges"))
    {
        const QJsonValue ranges = dataFromServerObj.value("Status Data Table Displayed Ranges");
        for (const QJsonValue& item : ranges.toArray())
        {
            std::pair<unsigned, unsigned> range = validateRange(item, static_cast<int>(inboundDataItems.size()));
            inboundDataTableRanges.push_back(range);
        }
    }
    else
    {
        inboundDataTableRanges.push_back({0, inboundDataItems.size()});
    }
}

void SystemDataSource::parseOutboundData()
{
    const QJsonValue dataItems = obj.value("Data To Server").toObject().value("Data Items");
    for (const QJsonValue& item : dataItems.toArray())
    {
        std::shared_ptr<DataItem> dataItem = std::make_shared<DataItem>(QString(item.toObject().value("Data Type").toString()),
                                                                        QString(item.toObject().value("Name").toString()),
                                                                        QString(item.toObject().value("Units").toString()),
                                                                        QString(item.toObject().value("Format").toString()));

        const QString defaultValString = item.toObject().value("Default Value").toVariant().toString();
        dataItem->setDefaultDisplayValue(defaultValString);

        outboundDataItems.push_back(dataItem);

        if(!defaultValString.isEmpty())
        {
            setOutboundDisplayValue(outboundDataItems.size() - 1, defaultValString);
        }

        const QString minValString = item.toObject().value("Min Value").toVariant().toString();
        const QString maxValString = item.toObject().value("Max Value").toVariant().toString();

        if(!minValString.isEmpty() && !maxValString.isEmpty())
        {
            (outboundDataItems[outboundDataItems.size() - 1])->setValueRange(convertDisplayToRawValue(dataItem->getDataItemType(), minValString),
                                                                             convertDisplayToRawValue(dataItem->getDataItemType(), maxValString));
        }
    }
}

void SystemDataSource::parseOutboundDataTableRanges()
{
    const QJsonObject dataToServerObj = obj.value("Data To Server").toObject();

    if(dataToServerObj.contains("Control Data Table Displayed Ranges"))
    {
        const QJsonValue ranges = dataToServerObj.value("Control Data Table Displayed Ranges");
        for (const QJsonValue& item : ranges.toArray())
        {
            std::pair<unsigned, unsigned> range = validateRange(item, static_cast<int>(outboundDataItems.size()));
            outboundDataTableRanges.push_back(range);
        }
    }
    else
    {
        outboundDataTableRanges.push_back({0, outboundDataItems.size()});
    }

}

QString SystemDataSource::convertRawToDisplayValue(const QString& type,
                                                   unsigned rawValue,
                                                   const QString& format) const
{
    if(rawValue == UINT_MAX)
    {
        return "ERROR";
    }

    if(type == TypeUnsignedInteger)
    {
        return QString::number(rawValue);
    }
    else if(type == TypeInteger)
    {
        return QString::number(unsignedToInt(rawValue));
    }
    else if(type == TypeNumeric)
    {
        return formatFloatDisplayValue(unsignedToFloat(rawValue), format);
    }
    else
    {
        // Data Type is an enum
        return getEnumStringValue(type, rawValue);
    }
}

unsigned SystemDataSource::convertDisplayToRawValue(const QString& type,
                                                    const QString& displayValue) const
{
    bool status = false;

    if(displayValue == "ERROR")
    {
        return UINT_MAX;
    }

    if(type == TypeUnsignedInteger)
    {
        unsigned intermediate = displayValue.toUInt(&status);
        if(status)
        {
            return intermediate;
        }
    }
    else if(type == TypeInteger)
    {
        int intermediate = displayValue.toInt(&status);
        if(status)
        {
            return intToUnsigned(intermediate);
        }
    }
    else if(type == TypeNumeric)
    {
        float intermediate = displayValue.toFloat(&status);
        if(status)
        {
            return floatToUnsigned(intermediate);
        }
    }
    else
    {
        // Data Type is an enum
        return getEnumUintValue(type, displayValue);
    }

    return UINT_MAX;
}

unsigned SystemDataSource::jsonStringToUnsigned(QString jsonValue)
{
    if(jsonValue.toLower().contains("x") || jsonValue.toLower().contains("X"))
    {
        bool status;
        return jsonValue.toUInt(&status, 16);
    }
    else
    {
        return jsonValue.toUInt();
    }
}

std::pair<unsigned, unsigned> SystemDataSource::validateRange(const QJsonValue& rangeItem, int maxIndex)
{
    int startIndex = rangeItem.toObject().value("Starting Index").toInt();

    int endIndex;
    (rangeItem.toObject().contains("Ending Index")) ? endIndex = rangeItem.toObject().value("Ending Index").toInt() :
                                                      endIndex = maxIndex;

    int trueStartIndex, trueEndIndex;

    // Validate the Starting Index:
    ((startIndex >= 0) && (startIndex < maxIndex)) ? trueStartIndex = startIndex : trueStartIndex = 0;

    // Validate the Ending Index:
    ((endIndex > trueStartIndex) && (endIndex <= maxIndex)) ? trueEndIndex = endIndex : trueEndIndex = maxIndex;

    return {trueStartIndex, trueEndIndex};
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
        return "ERROR";
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
        return UINT_MAX;
    }
}

void SystemDataSource::setInboundRawValues(const std::vector<unsigned>& rawValues)
{
    if(rawValues.size() == inboundDataItems.size())
    {
        for(unsigned i = 0; i < rawValues.size(); ++i)
        {
            inboundDataItems[i]->setRawValue(rawValues[i]);
            inboundDataItems[i]->setDisplayValue(convertRawToDisplayValue(inboundDataItems[i]->getDataItemType(),
                                                                          rawValues[i],
                                                                          inboundDataItems[i]->getDataItemFormat()));
        }
    }
}

std::vector<DataItem*> SystemDataSource::getInboundDataItems() const
{
    std::vector<DataItem*> dataItems = {};
    for(const auto& item : inboundDataItems)
    {
        dataItems.push_back(item.get());
    }

    return dataItems;
}

std::vector<QString> SystemDataSource::getInboundDataItemNames() const
{
    std::vector<QString> displayName = {};
    for(const auto& item : inboundDataItems)
    {
        displayName.push_back(item->getDataItemName());
    }

    return displayName;
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

std::vector<unsigned> SystemDataSource::getInboundRawValues() const
{
    std::vector<unsigned> rawValues = {};
    for(const auto& item : inboundDataItems)
    {
        rawValues.push_back(item->getRawValue());
    }

    return rawValues;
}

void SystemDataSource::setOutboundDisplayValue(unsigned index, const QString& displayValue)
{
    unsigned rawValue = convertDisplayToRawValue(outboundDataItems[index]->getDataItemType(), displayValue);
    outboundDataItems[index]->setRawValue(rawValue);
    outboundDataItems[index]->setDisplayValue(convertRawToDisplayValue(outboundDataItems[index]->getDataItemType(),
                                                                       rawValue,
                                                                       outboundDataItems[index]->getDataItemFormat()));
}

void SystemDataSource::setOutboundDisplayValues(const std::vector<QString>& displayValues)
{
    if(displayValues.size() == outboundDataItems.size())
    {
        for(unsigned i = 0; i < displayValues.size(); ++i)
        {
            const QString value = displayValues[i];
            if(!value.isEmpty()) setOutboundDisplayValue(i, value);
        }
    }
}

DataItem* SystemDataSource::getOutboundDataItem(int index) const
{
    return outboundDataItems[index].get();
}

std::vector<DataItem *> SystemDataSource::getOutboundDataItems() const
{
    std::vector<DataItem*> dataItems = {};
    for(const auto& item : outboundDataItems)
    {
        dataItems.push_back(item.get());
    }

    return dataItems;
}

std::vector<QString> SystemDataSource::getOutboundDataItemNames() const
{
    std::vector<QString> displayName = {};
    for(const auto& item : outboundDataItems)
    {
        displayName.push_back(item->getDataItemName());
    }

    return displayName;
}

std::vector<QString> SystemDataSource::getOutboundDisplayValues() const
{
    std::vector<QString> displayValues = {};
    for(const auto& item : outboundDataItems)
    {
        displayValues.push_back(item->getDisplayValue());
    }

    return displayValues;
}

std::vector<QString> SystemDataSource::getOutboundDefaultDisplayValues() const
{
    std::vector<QString> defaultValues = {};
    for(const auto& item : outboundDataItems)
    {
        defaultValues.push_back(item->getDefaultDisplayValue());
    }

    return defaultValues;
}

std::vector<unsigned> SystemDataSource::getOutboundRawValues() const
{
    std::vector<unsigned> rawValues = {};
    for(const auto& item : outboundDataItems)
    {
        rawValues.push_back(item->getRawValue());
    }

    return rawValues;
}

std::vector<std::pair<unsigned, unsigned>> SystemDataSource::getOutboundDataTableRanges() const
{
    return outboundDataTableRanges;
}
