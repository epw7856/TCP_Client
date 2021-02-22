#include "DataItem.h"
#include "DataUtilities.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QVariant>
#include "SemanticVersionInfo.h"
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

QString SystemDataSource::getConfigurationVersion() const
{
    return appSettings.configurationVersion;
}

QString SystemDataSource::getHeaderFooterText() const
{
    return appSettings.headerFooterText;
}

QString SystemDataSource::getSemanticVersion() const
{
    return SemanticVersion;
}

QString SystemDataSource::getApplicationName() const
{
    return ApplicationName;
}

QString SystemDataSource::getDepartmentName() const
{
    return DepartmentName;
}

QString SystemDataSource::getOrganizationName() const
{
    return OrganizationName;
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
    appSettings.configurationVersion = jsonAppSettings.toObject().value("Configuration Version").toVariant().toString();
    appSettings.headerFooterText = jsonAppSettings.toObject().value("Header Footer Text").toVariant().toString();
    appSettings.socketPort = jsonStringToUnsigned(jsonAppSettings.toObject().value("Socket Port").toVariant().toString());
    appSettings.transmissionPeriodicity = jsonStringToUnsigned(jsonAppSettings.toObject().value("Transmission Periodicity To Server (ms)").toVariant().toString());
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
    for(const QJsonValue& item : dataItems.toArray())
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
    for(const QJsonValue& item : dataItems.toArray())
    {
        std::shared_ptr<DataItem> dataItem = std::make_shared<DataItem>(QString(item.toObject().value("Data Type").toString()),
                                                                        QString(item.toObject().value("Name").toString()),
                                                                        QString(item.toObject().value("Units").toString()),
                                                                        QString(item.toObject().value("Format").toString()));

        QString defaultValString = item.toObject().value("Default Value").toVariant().toString();
        if(defaultValString.isEmpty())
        {
            defaultValString = convertRawToDisplayValue(dataItem->getDataItemType(), 0);
        }

        dataItem->setDefaultDisplayValue(defaultValString);
        outboundDataItems.push_back(dataItem);
        setOutboundDisplayValue(outboundDataItems.size() - 1, defaultValString);

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
    int startIndex = rangeItem.toObject().value("Starting Index (Inclusive)").toInt();

    int endIndex;
    (rangeItem.toObject().contains("Ending Index (Exclusive)")) ? endIndex = rangeItem.toObject().value("Ending Index (Exclusive)").toInt() :
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

DataItem *SystemDataSource::getInboundDataItem(int index) const
{
    return ((index < static_cast<int>(inboundDataItems.size())) ? inboundDataItems[index].get() : nullptr);
}

DataItem *SystemDataSource::getInboundDataItem(const QString& dataItemName) const
{
    auto itr = std::find_if(inboundDataItems.begin(), inboundDataItems.end(), [&](const std::shared_ptr<DataItem>& item){ return dataItemName == item->getDataItemName(); });
    return ((itr != inboundDataItems.end()) ? itr->get() : nullptr);
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

QString SystemDataSource::getInboundDataItemDisplayValue(int index) const
{
    DataItem* item = getInboundDataItem(index);
    return (item != nullptr) ? item->getDisplayValue() : QString();
}

QString SystemDataSource::getInboundDataItemDisplayValue(const QString& key) const
{
    DataItem* item = getInboundDataItem(key);
    return (item != nullptr) ? item->getDisplayValue() : QString();
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

std::vector<std::pair<unsigned, unsigned>> SystemDataSource::getInboundDataTableRanges() const
{
    return inboundDataTableRanges;
}

void SystemDataSource::setOutboundDisplayValue(unsigned index, const QString& displayValue)
{
    if(!displayValue.isEmpty())
    {
        unsigned rawValue = convertDisplayToRawValue(outboundDataItems[index]->getDataItemType(), displayValue);
        outboundDataItems[index]->setRawValue(rawValue);
        outboundDataItems[index]->setDisplayValue(convertRawToDisplayValue(outboundDataItems[index]->getDataItemType(),
                                                                           rawValue,
                                                                           outboundDataItems[index]->getDataItemFormat()));
    }
}

void SystemDataSource::setOutboundDisplayValue(const QString& key, const QString& displayValue)
{
    auto itr = std::find_if(outboundDataItems.begin(), outboundDataItems.end(), [&](const std::shared_ptr<DataItem>& item){ return key == item->getDataItemName(); });
    if(itr != outboundDataItems.end())
    {
        setOutboundDisplayValue(std::distance(outboundDataItems.begin(), itr), displayValue);
    }
}

void SystemDataSource::setOutboundDisplayValues(const std::vector<QString>& displayValues)
{
    if(displayValues.size() == outboundDataItems.size())
    {
        for(unsigned i = 0; i < displayValues.size(); ++i)
        {
            setOutboundDisplayValue(i, displayValues[i]);
        }
    }
}

DataItem* SystemDataSource::getOutboundDataItem(int index) const
{
    return ((index < static_cast<int>(outboundDataItems.size())) ? outboundDataItems[index].get() : nullptr);
}

DataItem *SystemDataSource::getOutboundDataItem(const QString& dataItemName) const
{
    auto itr = std::find_if(outboundDataItems.begin(), outboundDataItems.end(), [&](const std::shared_ptr<DataItem>& item){ return dataItemName == item->getDataItemName(); });
    return ((itr != outboundDataItems.end()) ? itr->get() : nullptr);
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

QString SystemDataSource::getOutboundDataItemDisplayValue(int index) const
{
    DataItem* item = getOutboundDataItem(index);
    return (item != nullptr) ? item->getDisplayValue() : QString();
}

QString SystemDataSource::getOutboundDataItemDisplayValue(const QString& key) const
{
    DataItem* item = getOutboundDataItem(key);
    return (item != nullptr) ? item->getDisplayValue() : QString();
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

QString SystemDataSource::getOutboundDefaultDisplayValue(int index) const
{
    return outboundDataItems[index]->getDefaultDisplayValue();
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
