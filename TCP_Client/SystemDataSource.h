#ifndef SYSTEMDATASOURCE_H
#define SYSTEMDATASOURCE_H

#include "ApplicationInterface.h"
#include "ApplicationSettings.h"
#include "EnumInterface.h"
#include "EnumType.h"
#include "InboundDataInterface.h"
#include <memory>
#include "OutboundDataInterface.h"
#include <QFile>
#include <QJsonObject>
#include <unordered_map>

class DataItem;

class SystemDataSource : public ApplicationInterface,
                         public EnumInterface,
                         public InboundDataInterface,
                         public OutboundDataInterface
{
public:
    SystemDataSource() {}

    // Public helper functions
    bool loadSystemConfiguration(const QString& configFilePath);

    // Implementation of Application Interface
    QString getSemanticVersion() const override;
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
    DataItem* getInboundDataItem(int index) const override;
    std::vector<DataItem*> getInboundDataItems() const override;
    std::vector<QString> getInboundDataItemNames() const override;
    std::vector<QString> getInboundDisplayValues() const override;
    std::vector<unsigned> getInboundRawValues() const override;
    std::vector<std::pair<unsigned, unsigned>> getInboundDataTableRanges() const override;
    DataItem* getReservedInboundDataItem(const QString& key) override;

    // Implementation of Outbound Data Interface
    void setOutboundDisplayValue(unsigned index, const QString& displayValue) override;
    void setOutboundDisplayValues(const std::vector<QString>& displayValues) override;
    DataItem* getOutboundDataItem(int index) const override;
    std::vector<DataItem*> getOutboundDataItems() const override;
    std::vector<QString> getOutboundDataItemNames() const override;
    std::vector<QString> getOutboundDisplayValues() const override;
    QString getOutboundDefaultDisplayValue(int index) const override;
    std::vector<unsigned> getOutboundRawValues() const override;
    std::vector<std::pair<unsigned, unsigned>> getOutboundDataTableRanges() const override;
    DataItem* getReservedOutboundDataItem(const QString& key) override;

private:
    QFile systemConfigFile;
    QJsonObject obj;

    ApplicationSettings appSettings;
    std::unordered_map<std::string, std::shared_ptr<EnumType>> enumRegistry = {};
    std::vector<std::shared_ptr<DataItem>> inboundDataItems = {};
    std::vector<std::shared_ptr<DataItem>> outboundDataItems = {};
    std::vector<std::pair<unsigned, unsigned>> inboundDataTableRanges = {};
    std::vector<std::pair<unsigned, unsigned>> outboundDataTableRanges = {};

    // Private helper functions for JSON file loading and parsing
    void clearSystemData();
    void parseApplicationSettings();
    void parseEnumerations();
    void parseInboundData();
    void parseInboundDataTableRanges();
    void parseOutboundData();
    void parseOutboundDataTableRanges();

    // Private helper functions for data conversions
    QString convertRawToDisplayValue(const QString& type,
                                     unsigned rawValue,
                                     const QString& format = QString()) const;

    unsigned convertDisplayToRawValue(const QString& type,
                                      const QString& displayValue) const;

    unsigned jsonStringToUnsigned(QString jsonValue);

    std::pair<unsigned, unsigned> validateRange(const QJsonValue& rangeItem, int maxIndex);

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
