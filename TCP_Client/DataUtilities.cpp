#include "DataUtilities.h"
#include <memory>

int unsignedToInt(unsigned value)
{
    int intValue;
    memcpy(&intValue, &value, sizeof (value));
    return intValue;
}

float unsignedToFloat(unsigned value)
{
    float fValue;
    memcpy(&fValue, &value, sizeof (value));
    return fValue;
}

unsigned intToUnsigned(int value)
{
    float uValue;
    memcpy(&uValue, &value, sizeof (value));
    return uValue;
}

unsigned floatToUnsigned(float value)
{
    unsigned uValue;
    memcpy(&uValue, &value, sizeof (value));
    return uValue;
}

QString formatFloatDisplayValue(float value, const QString &format)
{
    return (format.isEmpty()) ? QString::number(value) :
                                QString().asprintf(format.toStdString().c_str(), value);
}
