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

    const std::vector<QString>& getDesiredOutboundValues() const;
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
};

#endif // OUTBOUNDDATATABLEMODEL_H
