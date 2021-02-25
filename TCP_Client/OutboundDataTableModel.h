#ifndef OUTBOUNDDATATABLEMODEL_H
#define OUTBOUNDDATATABLEMODEL_H

#include <QAbstractTableModel>
#include "RangeCheckHandler.h"

class DataItem;
class EnumInterface;
class OutboundDataInterface;

class OutboundDataTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    OutboundDataTableModel(OutboundDataInterface& localOutboundDataInterface, EnumInterface& localEnumInterface);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    int getNewValueColumn() const;
    const std::vector<QString>& getDesiredOutboundValues() const;
    std::map<unsigned, DataItem*>& getOutboundDataItemMap();
    void setOutboundDataItems();
    void setDesiredOutboundValue(int index, QString value);
    void setDesiredOutboundValues(const std::vector<QString>& values);
    void applyDesiredOutboundValues();
    void resetDesiredOutboundValuesToDefaults();
    void clearDesiredOutboundValues();
    void clearDesiredOutboundValues(const std::vector<unsigned>& indices);

private:
    OutboundDataInterface& outboundDataInterface;
    RangeCheckHandler rangeChecker;
    std::map<unsigned, DataItem*> outboundDataItemMap = {};
    std::vector<QString> desiredOutboundValues = {};
    int numColumns = 5;
    int newValueColumn = 3;
};

inline int OutboundDataTableModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(outboundDataItemMap.size());
}

inline int OutboundDataTableModel::columnCount(const QModelIndex&) const
{
    return numColumns;
}

inline int OutboundDataTableModel::getNewValueColumn() const
{
    return newValueColumn;
}

#endif // OUTBOUNDDATATABLEMODEL_H
