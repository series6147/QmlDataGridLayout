#include "datagrid.h"
#include "datagridcolumn.h"
#include "datagridheaderitempresenter.h"
#include "datagridheaderpresenter.h"
#include "datagriditemlayout.h"

DataGridHeaderPresenter::DataGridHeaderPresenter(QQuickItem *parent) : QQuickItem(parent)
{
    m_dataGrid = NULL;
    m_itemsLayout = NULL;
}

DataGrid *DataGridHeaderPresenter::dataGrid() const
{
    return m_dataGrid;
}

void DataGridHeaderPresenter::arrange()
{
    auto layout = m_dataGrid->itemLayout();

    for (auto it = m_items.begin(); it != m_items.end(); ++it)
    {
        auto column = it.key();
        auto item = qobject_cast<QQuickItem*>(it.value());

        item->setProperty("itemX", layout->columnOffset(column->column()));
        item->setProperty("itemWidth", layout->columnSize(column->column(), column->columnSpan()));
    }
}

void DataGridHeaderPresenter::componentComplete()
{
    QQuickItem::componentComplete();

    m_itemsLayout = findChild<QQuickItem*>("__DATAGRIDHEADERROWLAYOUT__");
}

void DataGridHeaderPresenter::createLayout()
{
    for (auto item : m_items)
    {
        item->setParent(NULL);
        item->setParentItem(NULL);
        item->deleteLater();
    }

    m_items.clear();

    if (m_dataGrid != NULL)
    {
        QQmlComponent component(qmlEngine(this), "qrc:/DataGridHeaderItem.qml");

        for (auto column : m_dataGrid->columns())
        {
            auto item = qobject_cast<DataGridHeaderItemPresenter*>(component.create());
            auto root = item->findChild<QQuickItem*>("__DATAGRIDHEADERITEMLAYOUT__");

            m_items[column] = item;

            item->setColumn(column);
            item->setDataGrid(m_dataGrid);
            item->setItemRow(column->row());
            item->setItemRowSpan(column->rowSpan());
            item->setItemVisible(column->headerVisible());
            item->setParent(m_itemsLayout);
            item->setParentItem(m_itemsLayout);

            connect(root, &QQuickItem::widthChanged, this, &DataGridHeaderPresenter::itemWidthChanged);
        }
    }

    arrange();
}

void DataGridHeaderPresenter::itemWidthChanged()
{
    auto item = qobject_cast<QQuickItem*>(sender()->parent());

    if (m_dataGrid != NULL)
    {
        auto column = m_items.key(item);

        if (column)
        {
            auto layout = m_dataGrid->itemLayout();

            layout->setColumnSize(column, item->implicitWidth());
        }
    }
}

void DataGridHeaderPresenter::layoutChanged()
{
    arrange();
}

void DataGridHeaderPresenter::setDataGrid(DataGrid *dataGrid)
{
    if (m_dataGrid == dataGrid)
    {
        return;
    }

    if (m_dataGrid != NULL)
    {
        disconnect(m_dataGrid->itemLayout(), &DataGridItemLayout::layoutChanged, this, &DataGridHeaderPresenter::layoutChanged);
    }

    m_dataGrid = dataGrid;
    emit dataGridChanged(m_dataGrid);

    if (m_dataGrid != NULL)
    {
        connect(m_dataGrid->itemLayout(), &DataGridItemLayout::layoutChanged, this, &DataGridHeaderPresenter::layoutChanged);

        createLayout();
    }
}
