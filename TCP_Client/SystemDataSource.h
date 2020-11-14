#ifndef SYSTEMDATASOURCE_H
#define SYSTEMDATASOURCE_H

#include "ApplicationInterface.h"
#include "ApplicationSettings.h"
#include "EnumRegistryInterface.h"
#include "EnumType.h"
#include "InboundDataInterface.h"
#include <memory>
#include "OutboundDataInterface.h"
#include <QFile>
#include <QJsonObject>
#include <unordered_map>

class DataItem;

class SystemDataSource : public ApplicationInterface,
                         public EnumRegistryInterface,
                         public InboundDataInterface,
                         public OutboundDataInterface
{
public:
    SystemDataSource() {}

    // Public helper functions
    bool loadSystemConfig(const QString& configFilePath);

    // Implementation of Application Interface
    QString getSystemConfigFilePath() const override;
    unsigned getSocketPort() const override;
    unsigned getTransmissionPeriodicity() const override;
    void setSystemConfigFilePath(const QString& path) override;
    void setSocketPort(unsigned port) override;
    void setTransmissionPeriodicity(unsigned periodicity) override;

    // Implementation of Enum Registry Interface
    std::vector<QString> getEnumStrings(const QString& enumName) const override;
    QString getEnumStringValue(const QString& enumName, unsigned value) const override;
    unsigned getEnumUintValue(const QString& enumName, const QString& enumString) const override;

    // Implementation of Inbound Data Interface
    void setInboundRawValues(const std::vector<unsigned>& rawValues) override;
    std::vector<QString> getInboundDataItemNames() const override;
    std::vector<QString> getInboundDisplayValues() const override;
    std::vector<unsigned> getInboundRawValues() const override;

    // Implementation of Outbound Data Interface
    void setOutboundRawValues(const std::vector<unsigned>& rawValues) override;
    void setOutboundRawValue(unsigned index, unsigned rawValue) override;
    std::vector<QString> getOutboundDataItemNames() const override;
    std::vector<QString> getOutboundDisplayValues() const override;
    std::vector<unsigned> getOutboundRawValues() const override;

private:
    QFile systemConfigFile;
    QJsonObject obj;

    ApplicationSettings appSettings;
    std::unordered_map<std::string, std::shared_ptr<EnumType>> enumRegistry = {};
    std::vector<std::shared_ptr<DataItem>> inboundDataItems = {};
    std::vector<std::shared_ptr<DataItem>> outboundDataItems = {};

    // Private helper functions for JSON file loading and parsing
    void clearSystemData();
    void parseApplicationSettings();
    void parseEnumerations();
    void parseInboundData();
    void parseOutboundData();

    // Private helper functions for data conversions
    QString convertRawToDisplayValue(const QString& type,
                                     unsigned rawValue,
                                     const QString& format = QString()) const;
    unsigned convertDisplayToRawValue(const QString& type,
                                      const QString& displayValue) const;
    unsigned jsonStringToUInt(QString jsonValue);
};

inline QString SystemDataSource::getSystemConfigFilePath() const
{
    return appSettings.systemConfigFilePath;
}

inline unsigned SystemDataSource::getSocketPort() const
{
   return appSettings.socketPort;
}

inline unsigned SystemDataSource::getTransmissionPeriodicity() const
{
   return appSettings.transmissionPeriodicity;
}

inline void SystemDataSource::setSystemConfigFilePath(const QString& path)
{
    appSettings.systemConfigFilePath = path;
}

inline void SystemDataSource::setSocketPort(unsigned port)
{
    appSettings.socketPort = port;
}

inline void SystemDataSource::setTransmissionPeriodicity(unsigned periodicity)
{
    appSettings.transmissionPeriodicity = periodicity;
}

#endif // SYSTEMDATASOURCE_H
