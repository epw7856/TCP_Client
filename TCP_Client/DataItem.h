#ifndef DATAITEM_H
#define DATAITEM_H

#include <QString>

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
        type(itemType.toLower()),
        name(itemName),
        units(itemUnits),
        format(itemFormat) {}

    QString getDataItemType() const;
    QString getDataItemName() const;
    unsigned getRawValue() const;
    QString getDisplayValue() const;
    QString getDataItemUnits() const;
    QString getDataItemFormat() const;
    std::pair<unsigned, unsigned> getDataItemRange() const;
    bool isRangeCheckingEnabled() const;

    void setRawValue(unsigned updatedValue);
    void setDisplayValue(const QString& updatedValue);
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

inline QString DataItem::getDataItemUnits() const
{
    return units;
}

inline QString DataItem::getDataItemFormat() const
{
    return type;
}

inline std::pair<unsigned, unsigned> DataItem::getDataItemRange() const
{
    return {range.minValue, range.minValue};
}

inline bool DataItem::isRangeCheckingEnabled() const
{
    return rangeCheckEnable;
}

inline void DataItem::setRawValue(unsigned updatedValue)
{
    rawValue = updatedValue;
}

inline void DataItem::setDisplayValue(const QString& updatedValue)
{
    displayValue = updatedValue;
}

void DataItem::setValueRange(unsigned min, unsigned max)
{
    range.minValue = min;
    range.maxValue = max;
    rangeCheckEnable = range.minValue != range.maxValue;
}

#endif // DATAITEM_H
