#ifndef INBOUNDDATATABLEMODEL_H
#define INBOUNDDATATABLEMODEL_H

#include <QAbstractTableModel>

class DataItem;
class InboundDataInterface;

class InboundDataTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    InboundDataTableModel(InboundDataInterface& localInboundDataInterface);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setInboundDataItems();

private:
    InboundDataInterface& inboundDataInterface;
    std::map<unsigned, DataItem*> inboundDataItemMap = {};
    int numColumns = 4;
};

#endif // INBOUNDDATATABLEMODEL_H
