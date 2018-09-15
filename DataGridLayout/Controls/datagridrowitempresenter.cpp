#include "datagrid.h"
#include "datagridcolumn.h"
#include "datagriditemlayout.h"
#include "datagridrowitempresenter.h"

DataGridRowItemPresenter::DataGridRowItemPresenter(QQuickItem *parent) : QQuickItem(parent)
{
    m_column = NULL;
    m_dataGrid = NULL;
    m_itemRow = 0;
    m_itemRowSpan = 1;
    m_itemVisible = true;
    m_itemWidth = 0;
    m_itemX = 0;
    m_model = NULL;
    m_observableObject = NULL;
    m_rowIndex = 0;
}

bool DataGridRowItemPresenter::isSelected() const
{
    return m_dataGrid != NULL ? m_dataGrid->isRowSelected(m_rowIndex) : false;
}

bool DataGridRowItemPresenter::itemVisible() const
{
    return m_itemVisible;
}

DataGrid *DataGridRowItemPresenter::dataGrid() const
{
    return m_dataGrid;
}

DataGridColumn* DataGridRowItemPresenter::column() const
{
    return m_column;
}

int DataGridRowItemPresenter::itemRow() const
{
    return m_itemRow;
}

int DataGridRowItemPresenter::itemRowSpan() const
{
    return m_itemRowSpan;
}

int DataGridRowItemPresenter::itemX() const
{
    return m_itemX;
}

int DataGridRowItemPresenter::rowIndex() const
{
    return m_rowIndex;
}

QAbstractItemModel *DataGridRowItemPresenter::model() const
{
    return m_model;
}

QObject *DataGridRowItemPresenter::observableObject() const
{
    return m_observableObject;
}

qreal DataGridRowItemPresenter::itemWidth() const
{
    return m_itemWidth;
}

QVariant DataGridRowItemPresenter::getValue(int rowIndex, QString role)
{
    if (m_dataGrid->model()
        && m_model
        && rowIndex >= 0
        && rowIndex < m_model->rowCount())
    {
        auto roleIndex = m_dataGrid->model()->roleNames().key(role.toUtf8());

        return m_model ? m_model->data(m_model->index(rowIndex, 0), roleIndex) : QVariant();
    }
    return QVariant();
}

QVariant DataGridRowItemPresenter::modelData() const
{
    if (m_column != NULL && m_model != NULL)
    {
        auto roleIndex = m_column->roleIndex();

        return m_model ? m_model->data(m_model->index(m_rowIndex, 0), roleIndex) : QVariant();
    }
    return QVariant();
}

void DataGridRowItemPresenter::contentWidthChanged(qreal width)
{
    if (m_dataGrid != NULL)
    {
        auto layout = m_dataGrid->itemLayout();

        layout->setColumnSize(m_column, width);
    }
}

void DataGridRowItemPresenter::dataGridDestroyed()
{
    m_dataGrid = Q_NULLPTR;
}

void DataGridRowItemPresenter::sendEvent(QString eventName, QVariant value)
{
    if (m_dataGrid != NULL)
    {
        emit m_dataGrid->userEvent(eventName, value);
    }
}

void DataGridRowItemPresenter::setColumn(DataGridColumn* column)
{
    if (m_column == column)
        return;

    m_column = column;
    emit columnChanged(m_column);
    emit modelDataChanged();
}

void DataGridRowItemPresenter::setDataGrid(DataGrid *dataGrid)
{
    if (m_dataGrid == dataGrid)
        return;

    if (m_dataGrid != NULL)
    {
        disconnect(m_dataGrid, &DataGrid::destroyed, this, &DataGridRowItemPresenter::dataGridDestroyed);
        disconnect(m_dataGrid, &DataGrid::selectionChanged, this, &DataGridRowItemPresenter::isSelectedChanged);
    }

    m_dataGrid = dataGrid;
    emit dataGridChanged(m_dataGrid);

    if (m_dataGrid != NULL)
    {
        setModel(m_dataGrid->sortFilterProxyModel());

        connect(m_dataGrid, &DataGrid::destroyed, this, &DataGridRowItemPresenter::dataGridDestroyed);
        connect(m_dataGrid, &DataGrid::selectionChanged, this, &DataGridRowItemPresenter::isSelectedChanged);
    }

    emit isSelectedChanged();
    emit modelDataChanged();
}

void DataGridRowItemPresenter::setItemRow(int itemRow)
{
    if (m_itemRow == itemRow)
        return;

    m_itemRow = itemRow;
    emit itemRowChanged(m_itemRow);
}

void DataGridRowItemPresenter::setItemRowSpan(int itemRowSpan)
{
    if (m_itemRowSpan == itemRowSpan)
        return;

    m_itemRowSpan = itemRowSpan;
    emit itemRowSpanChanged(m_itemRowSpan);
}

void DataGridRowItemPresenter::setItemVisible(bool itemVisible)
{
    if (m_itemVisible == itemVisible)
        return;

    m_itemVisible = itemVisible;
    emit itemVisibleChanged(m_itemVisible);
}

void DataGridRowItemPresenter::setItemWidth(qreal itemWidth)
{
    if (m_itemWidth == itemWidth)
        return;

    m_itemWidth = itemWidth;
    emit itemWidthChanged(m_itemWidth);
}

void DataGridRowItemPresenter::setItemX(int itemX)
{
    if (m_itemX == itemX)
        return;

    m_itemX = itemX;
    emit itemXChanged(m_itemX);
}

void DataGridRowItemPresenter::setModel(QAbstractItemModel *model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit modelChanged(m_model);
    emit modelDataChanged();
}

void DataGridRowItemPresenter::setModelData(QVariant modelData)
{
    if (m_column != NULL && m_model != NULL)
    {
        auto roleIndex = m_column->roleIndex();

        m_model->setData(m_model->index(m_rowIndex, 0), modelData, roleIndex);

        emit modelDataChanged();
    }
}

void DataGridRowItemPresenter::setObservableObject(QObject *observableObject)
{
    if (m_observableObject == observableObject)
        return;

    if (m_observableObject != NULL
        && m_dataGrid->observablePropertyName() != "")
    {
        disconnect(m_observableObject, (m_dataGrid->observablePropertyName() + "Changed").toLocal8Bit(), this, "modelDataChanged");
    }

    m_observableObject = observableObject;
    emit observableObjectChanged(m_observableObject);

    if (m_observableObject != NULL
        && m_dataGrid->observablePropertyName() != "")
    {
        connect(m_observableObject, (m_dataGrid->observablePropertyName() + "Changed").toLocal8Bit(), this, "modelDataChanged");
    }
}

void DataGridRowItemPresenter::setRowIndex(int rowIndex)
{
    if (m_rowIndex == rowIndex)
        return;

    m_rowIndex = rowIndex;
    emit rowIndexChanged(m_rowIndex);

    emit isSelectedChanged();
    emit modelDataChanged();
}

void DataGridRowItemPresenter::setValue(int rowIndex, QString role, QVariant value)
{
    if (m_model != NULL)
    {
        auto roleIndex = m_dataGrid->model()->roleNames().key(role.toUtf8());

        m_model->setData(m_model->index(rowIndex, 0), value, roleIndex);

        emit modelDataChanged();
    }
}
