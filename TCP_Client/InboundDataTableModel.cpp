#include "DataItem.h"
#include "InboundDataInterface.h"
#include "InboundDataTableModel.h"

int InboundDataTableModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(inboundDataItems.size());
}

int InboundDataTableModel::columnCount(const QModelIndex&) const
{
    return numColumns;
}

QVariant InboundDataTableModel::data(const QModelIndex& index, int role) const
{
    int numRows = rowCount(index);

    if((role == Qt::DisplayRole) &&
       (index.row() < numRows) &&
       (index.column() < numColumns))
    {
        auto rowUint = static_cast<quint32>(index.row());

        // Parameter column
        if(index.column() == 0)
        {
            return inboundDataItems[rowUint]->getDataItemName();
        }
        // Value column
        else if(index.column() == 1)
        {
            return inboundDataItems[rowUint]->getDisplayValue();
        }
        // Units column
        else if(index.column() == 2)
        {
            return inboundDataItems[rowUint]->getDataItemUnits();
        }
    }

    if(role == Qt::TextAlignmentRole)
    {
        if((index.row() < numRows) &&
           (index.column() < numColumns))
        {
            return (index.column() == 0) ? Qt::AlignLeft : Qt::AlignCenter;
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
                return QString("Parameter");
            }
            else if(section == 1)
            {
                return QString("Value");
            }
            else if(section == 2)
            {
                return QString("Units");
            }
        }
    }
    return {};
}

void InboundDataTableModel::setInboundDataItems()
{
    inboundDataItems = inboundDataInterface.getInboundDataItems();
}
