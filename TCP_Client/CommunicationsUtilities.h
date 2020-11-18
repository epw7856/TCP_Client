#ifndef COMMUNICATIONSUTILITIES_H
#define COMMUNICATIONSUTILITIES_H

#include <QByteArray>
#include <QDataStream>
#include <vector>

std::vector<unsigned> toUint32Vector(const QByteArray& byteArray);
QByteArray toByteArray(const std::vector<unsigned>& uint32Vector);
QByteArray toByteArray(const std::vector<unsigned>& uint32Vector, QDataStream::ByteOrder specifiedByteOrder);
std::vector<unsigned> reverseEndian(const std::vector<unsigned>& dataIn);

#endif // COMMUNICATIONSUTILITIES_H
