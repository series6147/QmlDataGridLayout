#include "datagrid.h"
#include "datagridcolumn.h"
#include "datagridheaderitempresenter.h"
#include "datagridheaderpresenter.h"
#include "datagriditemlayout.h"

DataGridHeaderPresenter::DataGridHeaderPresenter(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    m_dataGrid = NULL;
    m_itemsLayout = NULL;
}

DataGrid *DataGridHeaderPresenter::dataGrid() const
{
    return m_dataGrid;
}

DataGridColumn *DataGridHeaderPresenter::highlightedColumn()
{
    for (auto it = m_items.begin(); it != m_items.end(); ++it)
    {
        auto column = it.key();
        auto item = qobject_cast<DataGridHeaderItemPresenter*>(it.value());

        if (item && item->highlighted())
        {
            return column;
        }
    }
    return Q_NULLPTR;
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
    for (auto item : m_itemsLayout->childItems())
    {
        item->setParent(NULL);
        item->setParentItem(NULL);
        item->deleteLater();
    }

    m_items.clear();

    if (m_dataGrid != NULL)
    {
        auto columns = m_dataGrid->columns();
        QQmlComponent itemComponent(qmlEngine(this), "qrc:/DataGridHeaderItem.qml");
        QSet<QString> objNames;

        qSort(columns.begin(), columns.end()
              , [](DataGridColumn* a, DataGridColumn* b)
        {
            if (a->rowSpan() < b->rowSpan())
            {
                return false;
            }
            else if (a->rowSpan() > b->rowSpan())
            {
                return true;
            }
            else if (a->row() < b->row())
            {
                return true;
            }
            else if (a->row() > b->row())
            {
                return false;
            }
            else
            {
                return a->column() < b->column();
            }
        });

        for (auto column : columns)
        {
            auto objName = QString("%1x%2").arg(column->row()).arg(column->rowSpan());

            if (!objNames.contains(objName))
            {
                auto item = qobject_cast<QQuickItem*>(itemComponent.create());

                item->setObjectName(objName);
                item->setProperty("itemRow", column->row());
                item->setProperty("itemRowSpan", column->rowSpan());
                item->setParent(m_itemsLayout);
                item->setParentItem(m_itemsLayout);

                objNames.insert(objName);
            }
        }

        QQmlComponent component(qmlEngine(this), "qrc:/DataGridHeaderItem.qml");

        for (auto column : columns)
        {
            auto item = qobject_cast<DataGridHeaderItemPresenter*>(component.create());
            auto parent = m_itemsLayout->findChild<QQuickItem*>(QString("%1x%2").arg(column->row()).arg(column->rowSpan()));

            m_items[column] = item;

            item->setDataGrid(m_dataGrid);
            item->setColumn(column);
            item->setItemRow(column->row());
            item->setItemRowSpan(column->rowSpan());
            item->setItemVisible(column->headerVisible());
            item->setParent(parent);
            item->setParentItem(parent);
        }
    }

    arrange();
}

void DataGridHeaderPresenter::highlightHeader(qreal x, qreal y)
{
    if (m_dataGrid)
    {
        auto columns = m_dataGrid->columns();

        for (auto column : columns)
        {
            auto parent = m_itemsLayout->findChild<QQuickItem*>(QString("%1x%2").arg(column->row()).arg(column->rowSpan()));

            if (parent)
            {
                for (auto child : parent->childItems())
                {
                    auto item = qobject_cast<DataGridHeaderItemPresenter*>(child);

                    if (item)
                    {
                        auto rect = child->mapRectToItem(m_dataGrid->dragLayout(), child->boundingRect());

                        item->setHighlighted(rect.contains(QPointF(x, y)));
                    }
                }
            }
        }
    }
}

void DataGridHeaderPresenter::layoutChanged()
{
    arrange();
}

void DataGridHeaderPresenter::paint(QPainter *painter)
{
    if (m_dataGrid != NULL)
    {
        auto interior = QRectF(0, 0, width(), height());
        auto color = QColor(m_dataGrid->headerBackground());

        painter->fillRect(interior, color);
    }
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
