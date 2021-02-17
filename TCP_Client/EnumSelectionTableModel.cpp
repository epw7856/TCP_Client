#include "EnumSelectionTableModel.h"

int EnumSelectionTableModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(enumStrings.size());
}

int EnumSelectionTableModel::columnCount(const QModelIndex&) const
{
    return 1;
}

QVariant EnumSelectionTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(index.row() < rowCount())
        {
            return enumStrings[static_cast<unsigned>(index.row())];
        }
    }

    if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignHCenter;
    }

    return {};
}

QVariant EnumSelectionTableModel::headerData(int, Qt::Orientation, int) const
{
    return {};
}

void EnumSelectionTableModel::setEnumStrings(const std::vector<QString> &localEnumStrings)
{
    std::copy_if(localEnumStrings.begin(), localEnumStrings.end(),
                 std::back_inserter(enumStrings),
                 [&](auto i){ return (i.length() > 0); });
}
