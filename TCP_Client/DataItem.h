#ifndef DATAITEM_H
#define DATAITEM_H

#include "DataType.h"
#include <QString>

class DataItem
{
public:
    DataItem
    (
        DataType itemType,
        const QString& itemName,
        const QString& itemUnits,
        const QString& itemFormat
    )
    :
        type(itemType),
        name(itemName),
        units(itemUnits),
        format(itemFormat) {}

    unsigned getRawValue() const;
    QString getDisplayValue() const;

    void setRawValue(unsigned value);
    bool setDisplayValue(const QString& value);

private:
    DataType type = DataType::UnsignedInteger;
    QString name = "";
    unsigned rawValue = 0U;
    QString displayValue = "???";
    QString units = "";
    QString format = "";
    bool performRangeChecking = false;
};

inline unsigned DataItem::getRawValue() const
{
    return rawValue;
}

inline QString DataItem::getDisplayValue() const
{
    return displayValue;
}

#endif // DATAITEM_H
