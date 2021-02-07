#ifndef INBOUNDDATAINTERFACE_H
#define INBOUNDDATAINTERFACE_H

#include <QString>
#include <vector>

class DataItem;

class InboundDataInterface
{
public:
    InboundDataInterface() {}
    virtual ~InboundDataInterface() = default;

    // Data from server API
    virtual void setInboundRawValues(const std::vector<unsigned>& rawValues) = 0;
    virtual DataItem* getInboundDataItem(int index) const = 0;
    virtual std::vector<DataItem*> getInboundDataItems() const = 0;
    virtual std::vector<QString> getInboundDataItemNames() const = 0;
    virtual std::vector<QString> getInboundDisplayValues() const = 0;
    virtual std::vector<unsigned> getInboundRawValues() const = 0;
    virtual std::vector<std::pair<unsigned, unsigned>> getInboundDataTableRanges() const = 0;
};

#endif // INBOUNDDATAINTERFACE_H
