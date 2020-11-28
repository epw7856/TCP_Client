#ifndef OUTBOUNDDATATABLEMODEL_H
#define OUTBOUNDDATATABLEMODEL_H

#include <QAbstractTableModel>

class DataItem;
class OutboundDataInterface;

class OutboundDataTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    OutboundDataTableModel(OutboundDataInterface& localOutboundDataInterface);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    OutboundDataInterface& outboundDataInterface;
    std::vector<DataItem*> outboundDataItems = {};
    std::vector<QString> newValues = {};
    int numColumns = 4;
};

#endif // OUTBOUNDDATATABLEMODEL_H
