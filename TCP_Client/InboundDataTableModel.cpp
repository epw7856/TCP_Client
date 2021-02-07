#include "DataItem.h"
#include "InboundDataInterface.h"
#include "InboundDataTableModel.h"

InboundDataTableModel::InboundDataTableModel(InboundDataInterface &localInboundDataInterface)
:
    inboundDataInterface(localInboundDataInterface)
{

}

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

        // Data item index column
        if(index.column() == 0)
        {
            return rowUint;
        }
        // Parameter column
        else if(index.column() == 1)
        {
            return inboundDataItems[rowUint]->getDataItemName();
        }
        // Value column
        else if(index.column() == 2)
        {
            return inboundDataItems[rowUint]->getDisplayValue();
        }
        // Units column
        else if(index.column() == 3)
        {
            return inboundDataItems[rowUint]->getDataItemUnits();
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
    inboundDataItems.clear();
    inboundDataItems = inboundDataInterface.getInboundDataItems();
    endResetModel();
}
