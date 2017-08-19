#include "datagrid.h"
#include "datagridcolumn.h"
#include "datagridheaderitempresenter.h"
#include "datagriditemlayout.h"

DataGridHeaderItemPresenter::DataGridHeaderItemPresenter(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    m_column = NULL;
    m_dataGrid = NULL;
    m_itemRow = 0;
    m_itemRowSpan = 1;
    m_itemVisible = true;
    m_itemWidth = 0;
    m_itemX = 0;
    m_model = NULL;
    m_originWidth = 0;
    m_originX = 0;
    m_resizeStarted = false;
    m_sortOrder = Qt::AscendingOrder;

    setFiltersChildMouseEvents(true);
}

bool DataGridHeaderItemPresenter::childMouseEventFilter(QQuickItem *item, QEvent *event)
{
    Q_UNUSED(item);

    auto mouseEvent = dynamic_cast<QMouseEvent*>(event);

    if (mouseEvent && m_dataGrid != NULL && m_resizeStarted)
    {
        auto layout = m_dataGrid->itemLayout();

        layout->resizeColumn(m_column, m_originWidth + QCursor::pos().x() - m_originX);
    }
    return false;
}

bool DataGridHeaderItemPresenter::itemVisible() const
{
    return m_itemVisible;
}

DataGrid *DataGridHeaderItemPresenter::dataGrid() const
{
    return m_dataGrid;
}

DataGridColumn *DataGridHeaderItemPresenter::column() const
{
    return m_column;
}

int DataGridHeaderItemPresenter::itemRow() const
{
    return m_itemRow;
}

int DataGridHeaderItemPresenter::itemRowSpan() const
{
    return m_itemRowSpan;
}

int DataGridHeaderItemPresenter::itemWidth() const
{
    return m_itemWidth;
}

int DataGridHeaderItemPresenter::itemX() const
{
    return m_itemX;
}

QSortFilterProxyModel *DataGridHeaderItemPresenter::model() const
{
    return m_model;
}

QString DataGridHeaderItemPresenter::color() const
{
    return m_dataGrid == NULL ? "#ddd" : m_dataGrid->headerBackground();
}

Qt::SortOrder DataGridHeaderItemPresenter::sortOrder() const
{
    return m_sortOrder;
}

QVariant DataGridHeaderItemPresenter::modelData() const
{
    if (m_column != NULL && m_model != NULL && m_model->sourceModel() != NULL)
    {
        auto roleIndex = m_column->roleIndex();

        return m_model ? m_model->sourceModel()->roleNames()[roleIndex] : QVariant();
    }
    return QVariant();
}

void DataGridHeaderItemPresenter::componentComplete()
{
    QQuickItem::componentComplete();

    m_resizeArea = findChild<QQuickItem*>("__DATAGRIDHEADERITEMRESIZEAREA__");
}

void DataGridHeaderItemPresenter::paint(QPainter *painter)
{
    if (m_dataGrid != NULL)
    {
        auto interior = QRect(0, 0, width(), height());
        auto color = QColor(m_dataGrid->headerBackground());

        painter->fillRect(interior, color);
    }
}

void DataGridHeaderItemPresenter::releseResize()
{
    if (m_resizeStarted)
    {
        m_resizeStarted = false;

        setKeepMouseGrab(false);
        unsetCursor();
    }
}

void DataGridHeaderItemPresenter::setDataGrid(DataGrid *dataGrid)
{
    if (m_dataGrid == dataGrid)
        return;

    if (m_dataGrid != NULL)
    {
        disconnect(m_dataGrid, &DataGrid::modelChanged, this, &DataGridHeaderItemPresenter::modelDataChanged);
        disconnect(m_dataGrid, SIGNAL(headerBackgroundChanged()), this, SLOT(update()));
    }

    m_dataGrid = dataGrid;
    emit dataGridChanged(m_dataGrid);
    emit colorChanged();

    if (m_dataGrid != NULL)
    {
        setModel(m_dataGrid->sortFilterProxyModel());

        connect(m_dataGrid, &DataGrid::modelChanged, this, &DataGridHeaderItemPresenter::modelDataChanged);
        connect(m_dataGrid, SIGNAL(headerBackgroundChanged()), this, SLOT(update()));
    }
}

void DataGridHeaderItemPresenter::setColumn(DataGridColumn *column)
{
    if (m_column == column)
        return;

    m_column = column;
    emit columnChanged(m_column);
    emit modelDataChanged();
}

void DataGridHeaderItemPresenter::setItemRow(int itemRow)
{
    if (m_itemRow == itemRow)
        return;

    m_itemRow = itemRow;
    emit itemRowChanged(m_itemRow);
}

void DataGridHeaderItemPresenter::setItemRowSpan(int itemRowSpan)
{
    if (m_itemRowSpan == itemRowSpan)
        return;

    m_itemRowSpan = itemRowSpan;
    emit itemRowSpanChanged(m_itemRowSpan);
}

void DataGridHeaderItemPresenter::setItemVisible(bool itemVisible)
{
    if (m_itemVisible == itemVisible)
        return;

    m_itemVisible = itemVisible;
    emit itemVisibleChanged(m_itemVisible);
}

void DataGridHeaderItemPresenter::setItemWidth(int itemWidth)
{
    if (m_itemWidth == itemWidth)
        return;

    m_itemWidth = itemWidth;
    emit itemWidthChanged(m_itemWidth);
}

void DataGridHeaderItemPresenter::setItemX(int itemX)
{
    if (m_itemX == itemX)
        return;

    m_itemX = itemX;
    emit itemXChanged(m_itemX);
}

void DataGridHeaderItemPresenter::setModel(QSortFilterProxyModel *model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit modelChanged(m_model);
    emit modelDataChanged();
}

void DataGridHeaderItemPresenter::setModelData(QVariant modelData)
{
    Q_UNUSED(modelData);
}

void DataGridHeaderItemPresenter::setSortOrder(Qt::SortOrder sortOrder)
{
    if (m_sortOrder == sortOrder)
        return;

    m_sortOrder = sortOrder;
    emit sortOrderChanged(m_sortOrder);
}

void DataGridHeaderItemPresenter::sortColumn()
{
    if (m_model != NULL)
    {
        m_model->setSortRole(m_column->roleIndex());
        m_model->sort(0, m_sortOrder);

        m_column->setSortActive(true);
        m_sortOrder = m_sortOrder == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder;
    }
}

void DataGridHeaderItemPresenter::startResize()
{
    if (m_resizeArea != NULL && !m_resizeStarted)
    {
        m_resizeStarted = true;
        m_originWidth = m_column->width().toReal();
        m_originX = QCursor::pos().x();

        setCursor(Qt::SplitHCursor);
        setKeepMouseGrab(true);
    }
}
