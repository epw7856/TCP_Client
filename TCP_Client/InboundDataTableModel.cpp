#include "DataItem.h"
#include "InboundDataInterface.h"
#include "InboundDataTableModel.h"

InboundDataTableModel::InboundDataTableModel(InboundDataInterface &localInboundDataInterface)
:
    inboundDataInterface(localInboundDataInterface)
{

}

QVariant InboundDataTableModel::data(const QModelIndex& index, int role) const
{
    int numRows = rowCount(index);

    if((role == Qt::DisplayRole) &&
       (index.row() < numRows) &&
       (index.column() < numColumns))
    {
        auto rowUint = static_cast<quint32>(index.row());
        auto itr = inboundDataItemMap.begin();
        std::advance(itr, rowUint);

        // Data item index column
        if(index.column() == 0)
        {
            return itr->first;
        }
        // Parameter column
        else if(index.column() == 1)
        {
            return itr->second->getDataItemName();
        }
        // Value column
        else if(index.column() == 2)
        {
            return itr->second->getDisplayValue();
        }
        // Units column
        else if(index.column() == 3)
        {
            return itr->second->getDataItemUnits();
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

QVariant InboundDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            if(section == 0)
            {
                return QString("Index");
            }
            else if(section == 1)
            {
                return QString("Parameter");
            }
            else if(section == 2)
            {
                return QString("Value");
            }
            else if(section == 3)
            {
                return QString("Units");
            }
        }
    }
    return {};
}

void InboundDataTableModel::setInboundDataItems()
{
    beginResetModel();

    inboundDataItemMap.clear();
    for(const auto& i : inboundDataInterface.getInboundDataTableRanges())
    {
        for(unsigned j = i.first; j < i.second; j++)
        {
            inboundDataItemMap[j] = inboundDataInterface.getInboundDataItem(j);
        }
    }

    endResetModel();
}
