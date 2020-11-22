#ifndef INBOUNDDATATABLEMODEL_H
#define INBOUNDDATATABLEMODEL_H

#include <QAbstractTableModel>

class DataItem;
class InboundDataInterface;

class InboundDataTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    InboundDataTableModel(InboundDataInterface& localInboundDataInterface) :
        inboundDataInterface(localInboundDataInterface) {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void setInboundDataItems();

private:
    InboundDataInterface& inboundDataInterface;
    std::vector<DataItem*> inboundDataItems = {};
    int numColumns = 3;
};

#endif // INBOUNDDATATABLEMODEL_H
