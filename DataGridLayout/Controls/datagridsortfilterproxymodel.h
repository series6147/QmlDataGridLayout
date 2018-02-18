#ifndef DATAGRIDSORTFILTERPROXYMODEL_H
#define DATAGRIDSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class DataGrid;
class DataGridSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    DataGridSortFilterProxyModel(DataGrid *dataGrid, QObject *parent = Q_NULLPTR);

private:
    DataGrid* m_dataGrid;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // DATAGRIDSORTFILTERPROXYMODEL_H
