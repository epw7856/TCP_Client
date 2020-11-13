#ifndef INBOUNDDATAINTERFACE_H
#define INBOUNDDATAINTERFACE_H

#include <QString>
#include <vector>

class InboundDataInterface
{
public:
    InboundDataInterface() {}
    virtual ~InboundDataInterface() = default;

    // Data From Server API
    virtual void setInboundRawValues(const std::vector<unsigned>& rawValues) = 0;
    virtual std::vector<QString> getInboundDisplayValues() const = 0;
};

#endif // INBOUNDDATAINTERFACE_H
