#include "DataItem.h"
#include "OutboundDataInterface.h"
#include "OutboundDataTableModel.h"

OutboundDataTableModel::OutboundDataTableModel
(
        OutboundDataInterface& localOutboundDataInterface,
        EnumInterface& localEnumInterface
)
:
    outboundDataInterface(localOutboundDataInterface),
    rangeChecker(localOutboundDataInterface, localEnumInterface)
{

}

QVariant OutboundDataTableModel::data(const QModelIndex& index, int role) const
{
    if(((role == Qt::DisplayRole) || (role == Qt::EditRole)) &&
       (index.row() < rowCount(index)) &&
       (index.column() < numColumns))
    {
        auto rowUint = static_cast<quint32>(index.row());
        auto itr = outboundDataItemMap.begin();
        std::advance(itr, rowUint);

        // Index column
        if(index.column() == 0)
        {
            return itr->first;
        }
        // Parameter column
        else if(index.column() == 1)
        {
            return itr->second->getDataItemName();
        }
        // Current Value column
        else if(index.column() == 2)
        {
            return itr->second->getDisplayValue();
        }
        // New Value column
        else if(index.column() == 3)
        {
            return desiredOutboundValues[rowUint];
        }
        // Units column
        else if(index.column() == 4)
        {
            return itr->second->getDataItemUnits();
        }
    }

    if(role == Qt::TextAlignmentRole)
    {
        if((index.row() < rowCount(index)) &&
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
    outboundDataItemMap.clear();
    for(const auto& i : outboundDataInterface.getOutboundDataTableRanges())
    {
        for(unsigned j = i.first; j < i.second; j++)
        {
            outboundDataItemMap[j] = outboundDataInterface.getOutboundDataItem(j);
        }
    }

    desiredOutboundValues.clear();
    desiredOutboundValues.resize(outboundDataItemMap.size());
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
    if(values.size() == outboundDataItemMap.size())
    {
        beginResetModel();
        desiredOutboundValues = std::move(values);
        endResetModel();
    }
}

void OutboundDataTableModel::applyDesiredOutboundValues()
{
    std::vector<unsigned> desiredOutboundValueIndices;
    for(const auto& i : outboundDataItemMap)
    {
        desiredOutboundValueIndices.push_back(i.first);
    }

    if(!rangeChecker.validateOutboundData(desiredOutboundValueIndices, desiredOutboundValues))
    {
        return;
    }

    for(unsigned i = 0; i < outboundDataItemMap.size(); i++)
    {
        auto itr = outboundDataItemMap.begin();
        std::advance(itr, i);
        outboundDataInterface.setOutboundDisplayValue(itr->first, desiredOutboundValues[i]);
    }

    clearDesiredOutboundValues();
}

void OutboundDataTableModel::clearDesiredOutboundValues()
{
    beginResetModel();
    desiredOutboundValues.clear();
    desiredOutboundValues.resize(outboundDataItemMap.size());
    endResetModel();
}

void OutboundDataTableModel::resetDesiredOutboundValuesToDefaults()
{
    clearDesiredOutboundValues();

    beginResetModel();
    for(unsigned i = 0; i < outboundDataItemMap.size(); i++)
    {
        auto itr = outboundDataItemMap.begin();
        std::advance(itr, i);
        desiredOutboundValues[i] = outboundDataInterface.getOutboundDefaultDisplayValue(itr->first);
    }
    endResetModel();
}

void OutboundDataTableModel::clearDesiredOutboundValues(const std::vector<unsigned>& indices)
{
    beginResetModel();
    std::for_each(indices.begin(), indices.end(), [&](const unsigned index){ desiredOutboundValues[index] = QString(); });
    endResetModel();
}
