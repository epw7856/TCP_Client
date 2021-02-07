#include "DataItem.h"
#include "DataUtilities.h"
#include "EnumInterface.h"
#include "OutboundDataInterface.h"
#include <QMessageBox>
#include <QValidator>
#include "RangeCheckHandler.h"

bool RangeCheckHandler::validateOutboundDataItem(int index, QString& value)
{
    DataItem* item = outboundDataInterface.getOutboundDataItem(index);
    std::pair<unsigned, unsigned> range = item->getDataItemRange();
    QString type = item->getDataItemType();

    bool status = false;
    if(type == TypeUnsignedInteger)
    {
        status = verifyUnsignedIntegerValue(value, range);
    }
    else if(type == TypeInteger)
    {
        status = verifyUnsignedIntegerValue(value, {unsignedToInt(range.first), unsignedToInt(range.second)});
    }
    else if(type == TypeNumeric)
    {
        status = verifyNumericValue(value, {unsignedToFloat(range.first), unsignedToFloat(range.second)});
    }
    else
    {
        status = verifyEnumValue(type, value);
    }

    if(!status)
    {
        showInvalidValueErrorPopup(item, value);
    }

    return status;
}

bool RangeCheckHandler::validateOutboundData(const std::vector<unsigned>& indices, std::vector<QString>& values)
{
    if(indices.size() != values.size())
    {
        return false;
    }

    for(quint32 i = 0; i < indices.size(); i++)
    {
        if(!values[i].isEmpty())
        {
            if(!validateOutboundDataItem(indices[i], values[i]))
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

void RangeCheckHandler::showInvalidValueErrorPopup(DataItem* item, const QString& value)
{
    QString type = item->getDataItemType();
    QString msg = "Invalid or out-of-range outbound value detected: \n\n";
    msg += "Name: " + item->getDataItemName() + "\n";
    msg += "Type: " + type + "\n";
    msg += "Desired Value: " + value + "\n";

    if(type == TypeUnsignedInteger)
    {
        msg += "Range: " + QString::number(item->getDataItemRange().first) +
               " to " + QString::number(item->getDataItemRange().second) + "\n";
    }
    else if(type == TypeInteger)
    {
        msg += "Range: " + QString::number(unsignedToInt(item->getDataItemRange().first)) +
               " to " + QString::number(unsignedToInt(item->getDataItemRange().second)) + "\n";
    }
    else if(type == TypeNumeric)
    {
        msg += "Range: " + QString::number(unsignedToFloat(item->getDataItemRange().first)) +
               " to " + QString::number(unsignedToFloat(item->getDataItemRange().second)) + "\n";
    }
    else
    {
        std::vector<QString> enumStrings = enumInterface.getEnumStrings(type);
        QString enumList = "";
        for(unsigned i = 0; i < enumStrings.size(); i++)
        {
            (i != enumStrings.size() - 1) ? enumList += enumStrings[i] + ", " :
                                            enumList += enumStrings[i];
        }

        msg += "Enum Values: " + enumList + "\n";
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("Invalid Outbound Value");
    msgBox.setText(msg);
    msgBox.setFont(QFont("Segoe UI", 10));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}
