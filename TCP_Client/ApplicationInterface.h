#ifndef APPLICATIONINTERFACE_H
#define APPLICATIONINTERFACE_H

#include <QString>

class ApplicationInterface
{
public:
    ApplicationInterface() {}
    virtual ~ApplicationInterface() = default;

    virtual QString getSystemConfigFilePath() const = 0;
    virtual unsigned getSocketPort() const = 0;
    virtual unsigned getTransmissionPeriodicity() const = 0;

    virtual void setSystemConfigFilePath(const QString& path) = 0;
    virtual void setSocketPort(unsigned port) = 0;
    virtual void setTransmissionPeriodicity(unsigned periodicity) = 0;
};

#endif // APPLICATIONINTERFACE_H
