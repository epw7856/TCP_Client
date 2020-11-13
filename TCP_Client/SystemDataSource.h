#ifndef SYSTEMDATASOURCE_H
#define SYSTEMDATASOURCE_H

#include "ApplicationInterface.h"
#include "ApplicationSettings.h"
#include "EnumRegistryInterface.h"
#include "EnumType.h"
#include "InboundDataInterface.h"
#include <memory>
#include "OutboundDataInterface.h"
#include <unordered_map>

class DataItem;

class SystemDataSource : public ApplicationInterface,
                         public EnumRegistryInterface,
                         public InboundDataInterface,
                         public OutboundDataInterface
{
public:
    SystemDataSource() {}

    // Public Helper Functions
    void initializeSystemConfig(const QString& configFilePath);

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
    std::vector<QString> getInboundDisplayValues() const override;

    // Implementation of Outbound Data Interface

private:
    ApplicationSettings appSettings;
    std::unordered_map<std::string, std::shared_ptr<EnumType>> enumRegistry = {};
    std::vector<std::shared_ptr<DataItem>> inboundDataItems = {};
    std::vector<std::shared_ptr<DataItem>> outboundDataItems = {};

    // Private Helper Functions
    void updateDataItemDisplayValue(DataItem& dataItem);
    void updateDataItemRawValue(DataItem& dataItem);
};

#endif // SYSTEMDATASOURCE_H
