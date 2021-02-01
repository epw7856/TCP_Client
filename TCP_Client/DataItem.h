#ifndef DATAITEM_H
#define DATAITEM_H

#include <QString>

const QString TypeUnsignedInteger = "Unsigned";
const QString TypeInteger = "Integer";
const QString TypeNumeric = "Numeric";

class DataItem
{
public:
    DataItem
    (
        const QString& itemType,
        const QString& itemName,
        const QString& itemUnits,
        const QString& itemFormat
    )
    :
        type(itemType),
        name(itemName),
        units(itemUnits),
        format(itemFormat) {}

    QString getDataItemType() const;
    QString getDataItemName() const;
    unsigned getRawValue() const;
    QString getDisplayValue() const;
    QString getDefaultDisplayValue() const;
    QString getDataItemUnits() const;
    QString getDataItemFormat() const;
    std::pair<unsigned, unsigned> getDataItemRange() const;
    bool isRangeCheckingEnabled() const;

    void setRawValue(unsigned value);
    void setDisplayValue(const QString& value);
    void setDefaultDisplayValue(const QString& value);
    void setValueRange(unsigned min, unsigned max);

private:

    struct DataItemRange
    {
        unsigned minValue = 0U;
        unsigned maxValue = 0U;
    };

    QString type = "";
    QString name = "";
    unsigned rawValue = 0U;
    QString displayValue = "???";
    QString defaultDisplayValue = "";
    QString units = "";
    QString format = "";
    DataItemRange range;
    bool rangeCheckEnable = false;
};

inline QString DataItem::getDataItemType() const
{
    return type;
}

inline QString DataItem::getDataItemName() const
{
    return name;
}

inline unsigned DataItem::getRawValue() const
{
    return rawValue;
}

inline QString DataItem::getDisplayValue() const
{
    return displayValue;
}

inline QString DataItem::getDefaultDisplayValue() const
{
    return defaultDisplayValue;
}

inline QString DataItem::getDataItemUnits() const
{
    return units;
}

inline QString DataItem::getDataItemFormat() const
{
    return format;
}

inline std::pair<unsigned, unsigned> DataItem::getDataItemRange() const
{
    return {range.minValue, range.maxValue};
}

inline bool DataItem::isRangeCheckingEnabled() const
{
    return rangeCheckEnable;
}

inline void DataItem::setRawValue(unsigned value)
{
    rawValue = value;
}

inline void DataItem::setDisplayValue(const QString& value)
{
    displayValue = value;
}

inline void DataItem::setDefaultDisplayValue(const QString& value)
{
    defaultDisplayValue = value;
}

inline void DataItem::setValueRange(unsigned min, unsigned max)
{
    range.minValue = min;
    range.maxValue = max;
    rangeCheckEnable = range.minValue != range.maxValue;
}

#endif // DATAITEM_H
