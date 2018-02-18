#include "datagrid.h"
#include "datagridsortfilterproxymodel.h"

DataGridSortFilterProxyModel::DataGridSortFilterProxyModel(DataGrid *dataGrid, QObject *parent) : QSortFilterProxyModel(parent)
{
    m_dataGrid = dataGrid;
}

bool DataGridSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    return m_dataGrid == Q_NULLPTR ? true : m_dataGrid->filterAcceptsRowHandler(source_row, source_parent);
}
