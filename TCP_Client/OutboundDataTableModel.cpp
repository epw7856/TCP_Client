#include "DataItem.h"
#include "OutboundDataInterface.h"
#include "OutboundDataTableModel.h"

OutboundDataTableModel::OutboundDataTableModel(OutboundDataInterface& localOutboundDataInterface, EnumInterface& localEnumInterface)
:
    outboundDataInterface(localOutboundDataInterface),
    rangeChecker(localOutboundDataInterface, localEnumInterface)
{

}

int OutboundDataTableModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(outboundDataItems.size());
}

int OutboundDataTableModel::columnCount(const QModelIndex&) const
{
    return numColumns;
}

QVariant OutboundDataTableModel::data(const QModelIndex& index, int role) const
{
    int numRows = rowCount(index);

    if(((role == Qt::DisplayRole) || (role == Qt::EditRole)) &&
       (index.row() < numRows) &&
       (index.column() < numColumns))
    {
        auto rowUint = static_cast<quint32>(index.row());

        // Index column
        if(index.column() == 0)
        {
            return rowUint;
        }
        // Parameter column
        else if(index.column() == 1)
        {
            return outboundDataItems[rowUint]->getDataItemName();
        }
        // Current Value column
        else if(index.column() == 2)
        {
            return outboundDataItems[rowUint]->getDisplayValue();
        }
        // New Value column
        else if(index.column() == 3)
        {
            return desiredOutboundValues[rowUint];
        }
        // Units column
        else if(index.column() == 4)
        {
            return outboundDataItems[rowUint]->getDataItemUnits();
        }
    }

    if(role == Qt::TextAlignmentRole)
    {
        if((index.row() < numRows) &&
           (index.column() < numColumns))
        {
            return (index.column() == 1) ? Qt::AlignVCenter : Qt::AlignCenter;
        }
    }

    return {};
}

bool OutboundDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if (!checkIndex(index))
        {
            return false;
        }
        setDesiredOutboundValue(index.row(), value.toString());
        return true;
    }
    return false;
}

QVariant OutboundDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            if(section == 0)
            {
                return QString("Index");
            }
            if(section == 1)
            {
                return QString("Parameter");
            }
            else if(section == 2)
            {
                return QString("Current Value");
            }
            else if(section == 3)
            {
                return QString("New Value");
            }
            else if(section == 4)
            {
                return QString("Units");
            }
        }
    }
    return {};
}

Qt::ItemFlags OutboundDataTableModel::flags(const QModelIndex& index) const
{
    if(index.column() == 3)
    {
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }
    else
    {
        return (QAbstractTableModel::flags(index) & ~Qt::ItemIsEditable);
    }
}

const std::vector<QString>& OutboundDataTableModel::getDesiredOutboundValues() const
{
    return desiredOutboundValues;
}

void OutboundDataTableModel::setOutboundDataItems()
{
    beginResetModel();
    outboundDataItems.clear();
    outboundDataItems = outboundDataInterface.getOutboundDataItems();

    desiredOutboundValues.clear();
    desiredOutboundValues.resize(outboundDataItems.size());
    endResetModel();
}

void OutboundDataTableModel::setDesiredOutboundValue(int index, QString value)
{
    if((index >= 0) &&
       (index <= static_cast<int>(desiredOutboundValues.size() - 1)))
    {
        beginResetModel();
        desiredOutboundValues[index] = value;
        endResetModel();
    }
}

void OutboundDataTableModel::setDesiredOutboundValues(const std::vector<QString>& values)
{
    if(values.size() == desiredOutboundValues.size())
    {
        beginResetModel();
        desiredOutboundValues = std::move(values);
        endResetModel();
    }
}

void OutboundDataTableModel::applyDesiredOutboundValues()
{
    if(!rangeChecker.validateOutboundData(desiredOutboundValues))
    {
        return;
    }

    outboundDataInterface.setOutboundDisplayValues(desiredOutboundValues);
    clearDesiredOutboundValues();
}

void OutboundDataTableModel::clearDesiredOutboundValues()
{
    beginResetModel();
    desiredOutboundValues.clear();
    desiredOutboundValues.resize(outboundDataItems.size());
    endResetModel();
}

void OutboundDataTableModel::resetDesiredOutboundValuesToDefaults()
{
    clearDesiredOutboundValues();

    beginResetModel();
    desiredOutboundValues = outboundDataInterface.getOutboundDefaultDisplayValues();
    endResetModel();
}

void OutboundDataTableModel::clearDesiredOutboundValues(const std::vector<unsigned>& indices)
{
    beginResetModel();
    std::for_each(indices.begin(), indices.end(), [&](const unsigned index){ desiredOutboundValues[index] = QString(); });
    endResetModel();
}
