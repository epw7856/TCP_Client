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
        const QString& itemFormat,
        const std::pair<QString, QString>& itemRange
    )
    :
        type(itemType),
        name(itemName),
        units(itemUnits),
        format(itemFormat),
        range(itemRange),
        rangeCheckEnable(!range.first.isEmpty() && !range.second.isEmpty()) {}

    QString getDataItemType() const;
    QString getDataItemName() const;
    unsigned getRawValue() const;
    QString getDisplayValue() const;
    QString getDataItemUnits() const;
    QString getDataItemFormat() const;
    std::pair<QString, QString> getDataItemRange() const;
    bool isRangeCheckingEnabled() const;

    void setRawValue(unsigned updatedValue);
    void setDisplayValue(const QString& updatedValue);

private:
    QString type = "";
    QString name = "";
    unsigned rawValue = 0U;
    QString displayValue = "???";
    QString units = "";
    QString format = "";
    std::pair<QString, QString> range = {};
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

inline std::pair<QString, QString> DataItem::getDataItemRange() const
{
    return range;
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

#endif // DATAITEM_H
