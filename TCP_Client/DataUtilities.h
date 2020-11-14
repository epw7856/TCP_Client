#ifndef DATAUTILITIES_H
#define DATAUTILITIES_H

#include <QString>

int unsignedToInt(unsigned value);
float unsignedToFloat(unsigned value);
unsigned intToUnsigned(int value);
unsigned floatToUnsigned(float value);
QString formatFloatDisplayValue(float value, const QString& format);

#endif // DATAUTILITIES_H
