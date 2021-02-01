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
    RangeCheckHandler(const OutboundDataInterface& localOutboundDataInterface, const EnumInterface& localEnumInterface) :
        outboundDataInterface(localOutboundDataInterface), enumInterface(localEnumInterface) {}

    bool validateOutboundDataItem(int index, QString& value);
    bool validateOutboundData(std::vector<QString>& values);

private:
    const OutboundDataInterface& outboundDataInterface;
    const EnumInterface& enumInterface;

    bool verifyUnsignedIntegerValue(QString& value, std::pair<unsigned, unsigned> range);
    bool verifyIntegerValue(QString& value, std::pair<int, int> range);
    bool verifyNumericValue(QString& value, std::pair<float, float> range);
    bool verifyEnumValue(const QString& enumName, const QString& value);
    void showInvalidValueErrorPopup(DataItem* item, const QString& value);
};

#endif // RANGECHECKHANDLER_H
