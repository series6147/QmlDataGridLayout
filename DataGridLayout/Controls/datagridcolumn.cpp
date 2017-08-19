#include "datagrid.h"
#include "datagridcolumn.h"

DataGridColumn::DataGridColumn(QObject *parent) : QObject(parent)
{
    m_column = 0;
    m_columnSpan = 1;
    m_dataGrid = NULL;
    m_headerDelegate = NULL;
    m_headerVisible = true;
    m_itemAlignment = Qt::AlignLeft;
    m_itemDelegate = NULL;
    m_itemVisible = true;
    m_maxWidth = INT_MAX;
    m_minWidth = 10;
    m_row = 0;
    m_rowSpan = 1;
    m_sizeMode = SizeMode::AutoSize;
    m_sortActive = false;
    m_sortEnabled = true;
}

bool DataGridColumn::headerVisible() const
{
    return m_headerVisible;
}

bool DataGridColumn::itemVisible() const
{
    return m_itemVisible;
}

bool DataGridColumn::sortActive() const
{
    return m_sortActive;
}

bool DataGridColumn::sortEnabled() const
{
    return m_sortEnabled;
}

DataGrid *DataGridColumn::dataGrid() const
{
    return m_dataGrid;
}

int DataGridColumn::column() const
{
    return m_column;
}

int DataGridColumn::columnSpan() const
{
    return m_columnSpan;
}

int DataGridColumn::maxWidth() const
{
    return m_maxWidth;
}

int DataGridColumn::minWidth() const
{
    return m_minWidth;
}

int DataGridColumn::roleIndex() const
{
    bool ok = false;
    int roleIndex = m_role.toInt(&ok);

    return ok ? roleIndex : m_dataGrid->sortFilterProxyModel()->roleNames().key(m_role.toString().toLocal8Bit());
}

int DataGridColumn::row() const
{
    return m_row;
}

int DataGridColumn::rowSpan() const
{
    return m_rowSpan;
}

QQmlComponent* DataGridColumn::headerDelegate() const
{
    return m_headerDelegate;
}

QQmlComponent* DataGridColumn::itemDelegate() const
{
    return m_itemDelegate;
}

Qt::Alignment DataGridColumn::itemAlignment() const
{
    return m_itemAlignment;
}

QVariant DataGridColumn::role() const
{
    return m_role;
}

QVariant DataGridColumn::width() const
{
    return m_width;
}

DataGridColumn::SizeMode DataGridColumn::sizeMode() const
{
    return m_sizeMode;
}

void DataGridColumn::setColumn(int column)
{
    if (m_column == column)
        return;

    m_column = column;
    emit columnChanged(m_column);
}

void DataGridColumn::setColumnSpan(int columnSpan)
{
    if (columnSpan < 1)
    {
        columnSpan = 1;
    }

    if (m_columnSpan == columnSpan)
        return;

    m_columnSpan = columnSpan;
    emit columnSpanChanged(m_columnSpan);
}

void DataGridColumn::setDataGrid(DataGrid *dataGrid)
{
    if (m_dataGrid == dataGrid)
        return;

    m_dataGrid = dataGrid;
    emit dataGridChanged(m_dataGrid);
}

void DataGridColumn::setHeaderDelegate(QQmlComponent *headerDelegate)
{
    if (m_headerDelegate == headerDelegate)
        return;

    m_headerDelegate = headerDelegate;
    emit headerDelegateChanged(m_headerDelegate);
}

void DataGridColumn::setHeaderVisible(bool headerVisible)
{
    if (m_headerVisible == headerVisible)
        return;

    m_headerVisible = headerVisible;
    emit headerVisibleChanged(m_headerVisible);
}

void DataGridColumn::setMaxWidth(int maxWidth)
{
    if (m_maxWidth == maxWidth)
        return;

    m_maxWidth = maxWidth;
    emit maxWidthChanged(m_maxWidth);
}

void DataGridColumn::setMinWidth(int minWidth)
{
    if (m_minWidth == minWidth)
        return;

    m_minWidth = minWidth;
    emit minWidthChanged(m_minWidth);
}

void DataGridColumn::setItemAlignment(Qt::Alignment itemAlignment)
{
    if (m_itemAlignment == itemAlignment)
        return;

    m_itemAlignment = itemAlignment;
    emit itemAlignmentChanged(m_itemAlignment);
}

void DataGridColumn::setItemDelegate(QQmlComponent *itemDelegate)
{
    if (m_itemDelegate == itemDelegate)
        return;

    m_itemDelegate = itemDelegate;
    emit itemDelegateChanged(m_itemDelegate);
}

void DataGridColumn::setItemVisible(bool itemVisible)
{
    if (m_itemVisible == itemVisible)
        return;

    m_itemVisible = itemVisible;
    emit itemVisibleChanged(m_itemVisible);
}

void DataGridColumn::setRole(QVariant role)
{
    if (m_role == role)
        return;

    m_role = role;
    emit roleChanged(m_role);
}

void DataGridColumn::setRow(int row)
{
    if (m_row == row)
        return;

    m_row = row;
    emit rowChanged(m_row);
}

void DataGridColumn::setRowSpan(int rowSpan)
{
    if (rowSpan < 1)
    {
        rowSpan = 1;
    }

    if (m_rowSpan == rowSpan)
        return;

    m_rowSpan = rowSpan;
    emit rowSpanChanged(m_rowSpan);
}

void DataGridColumn::setSizeMode(DataGridColumn::SizeMode sizeMode)
{
    if (m_sizeMode == sizeMode)
        return;

    m_sizeMode = sizeMode;
    emit sizeModeChanged(m_sizeMode);
}

void DataGridColumn::setSortActive(bool sortActive)
{
    if (m_sortActive == sortActive)
        return;

    m_sortActive = sortActive;
    emit sortActiveChanged(m_sortActive);

    if (m_dataGrid != NULL && m_sortActive)
    {
        for (auto column : m_dataGrid->columns())
        {
            if (column != this)
            {
                column->setSortActive(false);
            }
        }
    }
}

void DataGridColumn::setSortEnabled(bool sortEnabled)
{
    if (m_sortEnabled == sortEnabled)
        return;

    m_sortEnabled = sortEnabled;
    emit sortEnabledChanged(m_sortEnabled);
}

void DataGridColumn::setWidth(QVariant width)
{
    if (m_width == width)
        return;

    m_width = width;
    emit widthChanged(m_width);
}
