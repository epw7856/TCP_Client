#include "DataItem.h"
#include "DataUtilities.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include "SystemDataSource.h"

bool SystemDataSource::loadSystemConfig(const QString& configFilePath)
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

    clearSystemData();
    parseApplicationSettings();
    parseEnumerations();
    parseInboundData();
    parseOutboundData();

    return true;
}

void SystemDataSource::clearSystemData()
{
    enumRegistry.clear();
    inboundDataItems.clear();
    outboundDataItems.clear();
}

void SystemDataSource::parseApplicationSettings()
{
    QJsonValue jsonAppSettings = obj.value("Application Settings");
    appSettings.socketPort = jsonStringToUInt(jsonAppSettings.toObject().value("Socket Port").toVariant().toString());
    appSettings.transmissionPeriodicity = jsonStringToUInt(jsonAppSettings.toObject().value("Transmission Periodicity (ms)").toVariant().toString());
}

void SystemDataSource::parseEnumerations()
{
    QJsonValue jsonEnums = obj.value("Enumerations");
    for (const QJsonValue& item : jsonEnums.toArray())
    {
        const QString setName = QString(item.toObject().value("Set Name").toString());
        std::shared_ptr<EnumType> enumSet = std::make_shared<EnumType>(setName);

        QJsonArray enumNames = item.toObject().value("Enum Names").toArray();
        for(const QJsonValue& name : enumNames)
        {
            if(name.toObject().contains("Value"))
            {
                unsigned value = appSettings.socketPort = jsonStringToUInt(name.toObject().value("Value").toVariant().toString());
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
    QJsonValue jsonDataFromServer = obj.value("Data From Server");
    for (const QJsonValue& item : jsonDataFromServer.toArray())
    {
        std::shared_ptr<DataItem> dataItem = std::make_shared<DataItem>(QString(item.toObject().value("Data Type").toString()),
                                                                        QString(item.toObject().value("Name").toString()),
                                                                        QString(item.toObject().value("Units").toString()),
                                                                        QString(item.toObject().value("Format").toString()));
        inboundDataItems.push_back(dataItem);
    }
}

void SystemDataSource::parseOutboundData()
{

}

QString SystemDataSource::convertRawToDisplayValue(const QString& type,
                                                   unsigned rawValue,
                                                   const QString& format) const
{
    if(rawValue == UINT_MAX)
    {
        return "ERROR";
    }

    if(type == "unsigned")
    {
        return QString::number(rawValue);
    }
    else if(type == "integer")
    {
        return QString::number(unsignedToInt(rawValue));
    }
    else if(type == "float")
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

    if(type == "unsigned")
    {
        unsigned intermediate = displayValue.toUInt(&status);
        if(status)
        {
            return intermediate;
        }
    }
    else if(type == "integer")
    {
        int intermediate = displayValue.toInt(&status);
        if(status)
        {
            return intToUnsigned(intermediate);
        }
    }
    else if(type == "float")
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

unsigned SystemDataSource::jsonStringToUInt(QString jsonValue)
{
    if(jsonValue.contains("."))
    {
        return floatToUnsigned(jsonValue.toFloat());
    }
    else if(jsonValue.toLower().contains("x"))
    {
        bool status;
        return jsonValue.toUInt(&status, 16);
    }
    else
    {
        return jsonValue.toUInt();
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

void SystemDataSource::setOutboundRawValues(const std::vector<unsigned>& rawValues)
{
    if(rawValues.size() == inboundDataItems.size())
    {
        for(unsigned i = 0; i < rawValues.size(); ++i)
        {
            outboundDataItems[i]->setRawValue(rawValues[i]);
            outboundDataItems[i]->setDisplayValue(convertRawToDisplayValue(outboundDataItems[i]->getDataItemType(),
                                                                           rawValues[i],
                                                                           outboundDataItems[i]->getDataItemFormat()));
        }
    }
}

void SystemDataSource::setOutboundRawValue(unsigned index, unsigned rawValue)
{
    outboundDataItems[index]->setRawValue(rawValue);
    outboundDataItems[index]->setDisplayValue(convertRawToDisplayValue(outboundDataItems[index]->getDataItemType(),
                                                                       rawValue,
                                                                       outboundDataItems[index]->getDataItemFormat()));
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

std::vector<unsigned> SystemDataSource::getOutboundRawValues() const
{
    std::vector<unsigned> rawValues = {};
    for(const auto& item : outboundDataItems)
    {
        rawValues.push_back(item->getRawValue());
    }

    return rawValues;
}
