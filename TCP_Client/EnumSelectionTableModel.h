#ifndef ENUMSELECTIONTABLEMODEL_H
#define ENUMSELECTIONTABLEMODEL_H

#include <QAbstractTableModel>

class EnumSelectionTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    EnumSelectionTableModel() {}
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void setEnumStrings(const std::vector<QString>& localEnumStrings);

private:
    std::vector<QString> enumStrings;
};

#endif // ENUMSELECTIONTABLEMODEL_H
