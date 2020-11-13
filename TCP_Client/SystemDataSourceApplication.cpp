#include "SystemDataSource.h"

QString SystemDataSource::getSystemConfigFilePath() const
{
    return appSettings.systemConfigFilePath;
}

unsigned SystemDataSource::getSocketPort() const
{
   return appSettings.socketPort;
}

unsigned SystemDataSource::getTransmissionPeriodicity() const
{
   return appSettings.transmissionPeriodicity;
}

void SystemDataSource::setSystemConfigFilePath(const QString& path)
{
    appSettings.systemConfigFilePath = path;
}

void SystemDataSource::setSocketPort(unsigned port)
{
    appSettings.socketPort = port;
}

void SystemDataSource::setTransmissionPeriodicity(unsigned periodicity)
{
    appSettings.transmissionPeriodicity = periodicity;
}
