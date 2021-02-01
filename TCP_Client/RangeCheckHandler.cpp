#include "DataItem.h"
#include "DataUtilities.h"
#include "EnumInterface.h"
#include "OutboundDataInterface.h"
#include <QValidator>
#include "RangeCheckHandler.h"

bool RangeCheckHandler::validateOutboundDataItem(int index, QString& value)
{
    DataItem* item = outboundDataInterface.getOutboundDataItem(index);
    std::pair<unsigned, unsigned> range = item->getDataItemRange();
    QString type = item->getDataItemType();

    if(type == TypeUnsignedInteger)
    {
        return verifyUnsignedIntegerValue(value, range);
    }
    else if(type == TypeInteger)
    {
        return verifyUnsignedIntegerValue(value, {unsignedToInt(range.first), unsignedToInt(range.second)});
    }
    else if(type == TypeNumeric)
    {
        return verifyNumericValue(value, {unsignedToFloat(range.first), unsignedToFloat(range.second)});
    }
    else
    {
        return verifyEnumValue(type, value);
    }

    return false;
}

bool RangeCheckHandler::validateOutboundData(std::vector<QString>& values)
{
    for(quint32 i = 0; i < values.size(); i++)
    {
        if(!values[i].isEmpty())
        {
            if(!validateOutboundDataItem(i, values[i]))
            {
                return false;
            }
        }
    }
    return true;
}

bool RangeCheckHandler::verifyUnsignedIntegerValue(QString& value, std::pair<unsigned, unsigned> range)
{
    bool status;
    value.toUInt(&status);
    if(!status)
    {
        return false;
    }

    if(range.first != range.second)
    {
        QIntValidator validator(range.first, range.second);
        QValidator::State state = QValidator::Invalid;

        int pos = 0;
        state = validator.validate(value, pos);
        return (state == QValidator::Acceptable);
    }
    return true;
}

bool RangeCheckHandler::verifyIntegerValue(QString& value, std::pair<int, int> range)
{
    bool status;
    value.toInt(&status);
    if(!status)
    {
        return false;
    }

    if(range.first != range.second)
    {
        QIntValidator validator(range.first, range.second);
        QValidator::State state = QValidator::Invalid;

        int pos = 0;
        state = validator.validate(value, pos);
        return (state == QValidator::Acceptable);
    }
    return true;
}

bool RangeCheckHandler::verifyNumericValue(QString& value, std::pair<float, float> range)
{
    bool status;
    value.toDouble(&status);
    if(!status)
    {
        return false;
    }

    const unsigned FloatPrecision = 7U;

    if(range.first != range.second)
    {
        QDoubleValidator validator(range.first, range.second, FloatPrecision);
        QValidator::State state = QValidator::Invalid;

        int pos = 0;
        state = validator.validate(value, pos);
        return (state == QValidator::Acceptable);
    }
    return true;
}

bool RangeCheckHandler::verifyEnumValue(const QString& enumName, const QString& value)
{
    std::vector<QString> enumStrings = enumInterface.getEnumStrings(enumName);
    auto itr = std::find_if(enumStrings.begin(), enumStrings.end(), [&](const QString& elem){ return elem == value; });
    return (itr != enumStrings.end());
}
