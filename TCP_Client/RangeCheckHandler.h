#ifndef RANGECHECKHANDLER_H
#define RANGECHECKHANDLER_H

#include <QString>
#include <vector>

class DataItem;
class EnumInterface;
class OutboundDataInterface;

class RangeCheckHandler
{
public:
    RangeCheckHandler(const OutboundDataInterface& localOutboundDataInterface) :
        outboundData(localOutboundDataInterface) {}

    bool validateOutboundData(int index, const QString& value);
    bool validateOutboundData(const std::vector<int, QString>& values);

private:
    const OutboundDataInterface& outboundData;

    bool verifyUnsignedIntegerValue(const QString& value, std::pair<unsigned, unsigned>& range);
    bool verifyIntegerValue(const QString& value, std::pair<int, int>& range);
    bool verifyNumericValue(const QString& value, std::pair<float, float>& range);
    bool verifyEnumValue(const QString& enumName, const QString& value);
};

#endif // RANGECHECKHANDLER_H
