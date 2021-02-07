#ifndef OUTBOUNDDATAINTERFACE_H
#define OUTBOUNDDATAINTERFACE_H

#include <QString>
#include <vector>

class DataItem;

class OutboundDataInterface
{
public:
    OutboundDataInterface() {}
    virtual ~OutboundDataInterface() = default;

    // Data to server API
    virtual void setOutboundDisplayValue(unsigned index, const QString& displayValue) = 0;
    virtual void setOutboundDisplayValues(const std::vector<QString>& displayValues) = 0;
    virtual DataItem* getOutboundDataItem(int index) const = 0;
    virtual std::vector<DataItem*> getOutboundDataItems() const = 0;
    virtual std::vector<QString> getOutboundDataItemNames() const = 0;
    virtual std::vector<QString> getOutboundDisplayValues() const = 0;
    virtual QString getOutboundDefaultDisplayValue(int index) const = 0;
    virtual std::vector<unsigned> getOutboundRawValues() const = 0;
    virtual std::vector<std::pair<unsigned, unsigned>> getOutboundDataTableRanges() const = 0;
};
#endif // OUTBOUNDDATAINTERFACE_H
