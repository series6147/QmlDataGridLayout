#include "datagrid.h"
#include "datagridrowpresenter.h"
#include "datagriditemlayout.h"
#include "datagridcolumn.h"
#include "datagridrowitempresenter.h"

DataGridRowPresenter::DataGridRowPresenter(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    m_dataGrid = NULL;
    m_itemRow = 0;
    m_itemsLayout = NULL;
    m_observableObject = NULL;

    setAcceptedMouseButtons(Qt::AllButtons);
    setFiltersChildMouseEvents(true);
}

DataGrid *DataGridRowPresenter::dataGrid() const
{
    return m_dataGrid;
}

int DataGridRowPresenter::itemRow() const
{
    return m_itemRow;
}

QObject *DataGridRowPresenter::observableObject() const
{
    return m_observableObject;
}

void DataGridRowPresenter::arrange()
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

void DataGridRowPresenter::componentComplete()
{
    QQuickItem::componentComplete();

    m_itemsLayout = findChild<QQuickItem*>("__DATAGRIDROWLAYOUT__");
}

void DataGridRowPresenter::createLayout()
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
        QQmlComponent component(qmlEngine(this), "qrc:/DataGridRowItem.qml");

        for (auto column : m_dataGrid->columns())
        {
            auto item = qobject_cast<DataGridRowItemPresenter*>(component.create());
            auto root = item->findChild<QQuickItem*>("__DATAGRIDROWITEMLAYOUT__");

            m_items[column] = item;

            item->setColumn(column);
            item->setDataGrid(m_dataGrid);
            item->setItemRow(column->row());
            item->setItemRowSpan(column->rowSpan());
            item->setItemVisible(column->itemVisible());
            item->setParent(m_itemsLayout);
            item->setParentItem(m_itemsLayout);
            item->setRowIndex(m_itemRow);

            connect(root, &QQuickItem::widthChanged, this, &DataGridRowPresenter::itemWidthChanged);
        }
    }

    arrange();
}

void DataGridRowPresenter::itemWidthChanged()
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

void DataGridRowPresenter::layoutChanged()
{
    arrange();
}

void DataGridRowPresenter::mousePressEvent(QMouseEvent *event)
{
    if (m_dataGrid != NULL)
    {
        m_dataGrid->setFocus(true);
        m_dataGrid->selectRow(m_itemRow, event);
    }
}

bool DataGridRowPresenter::childMouseEventFilter(QQuickItem *item, QEvent *event)
{
    Q_UNUSED(item);

    auto mouseEvent = dynamic_cast<QMouseEvent*>(event);

    if (mouseEvent)
    {
        m_dataGrid->selectRow(m_itemRow, mouseEvent);
    }
    return false;
}

void DataGridRowPresenter::paint(QPainter *painter)
{
    if (m_dataGrid != NULL)
    {
        auto interior = QRect(0, 0, width(), height());
        auto color = QColor(m_dataGrid->isRowSelected(m_itemRow)
                            ? m_dataGrid->hasFocus()
                              ? m_dataGrid->highlightColor()
                              : m_dataGrid->inactiveHighlightColor()
                            : m_itemRow % 2
                              ? m_dataGrid->alternativeRowBackground()
                              : "#fff");

        painter->fillRect(interior, color);
    }
}

void DataGridRowPresenter::parentFocusChanged()
{
    if (m_dataGrid != NULL && m_dataGrid->isRowSelected(m_itemRow))
    {
        update();
    }
}

void DataGridRowPresenter::refresh()
{
    for (auto it = m_items.begin(); it != m_items.end(); ++it)
    {
        auto item = qobject_cast<DataGridRowItemPresenter*>(it.value());

        if (item)
        {
            emit item->modelDataChanged();
        }
    }
}

void DataGridRowPresenter::setDataGrid(DataGrid *dataGrid)
{
    if (m_dataGrid == dataGrid)
    {
        return;
    }

    if (m_dataGrid != NULL)
    {
        disconnect(m_dataGrid, &DataGrid::focusChanged, this, &DataGridRowPresenter::parentFocusChanged);
        disconnect(m_dataGrid, SIGNAL(alternativeRowBackgroundChanged()), this, SLOT(update()));
        disconnect(m_dataGrid, SIGNAL(selectionChanged()), this, SLOT(update()));
        disconnect(m_dataGrid->itemLayout(), &DataGridItemLayout::layoutChanged, this, &DataGridRowPresenter::layoutChanged);
    }

    m_dataGrid = dataGrid;
    emit dataGridChanged(m_dataGrid);

    if (m_dataGrid != NULL)
    {
        connect(m_dataGrid, &DataGrid::focusChanged, this, &DataGridRowPresenter::parentFocusChanged);
        connect(m_dataGrid, SIGNAL(alternativeRowBackgroundChanged()), this, SLOT(update()));
        connect(m_dataGrid, SIGNAL(selectionChanged()), this, SLOT(update()));
        connect(m_dataGrid->itemLayout(), &DataGridItemLayout::layoutChanged, this, &DataGridRowPresenter::layoutChanged);

        setObservableObject(m_dataGrid->getObservableObject(m_itemRow));

        createLayout();
    }
}

void DataGridRowPresenter::setItemRow(int itemRow)
{
    if (m_itemRow == itemRow)
        return;

    m_itemRow = itemRow;
    emit itemRowChanged(m_itemRow);

    for (auto it = m_items.begin(); it != m_items.end(); ++it)
    {
        auto item = qobject_cast<DataGridRowItemPresenter*>(it.value());

        if (item)
        {
            item->setRowIndex(m_itemRow);
        }
    }

    if (m_dataGrid != NULL)
    {
        setObservableObject(m_dataGrid->getObservableObject(m_itemRow));
    }
}

void DataGridRowPresenter::setObservableObject(QObject *observableObject)
{
    if (m_observableObject == observableObject)
        return;

    m_observableObject = observableObject;
    emit observableObjectChanged(m_observableObject);

    for (auto it = m_items.begin(); it != m_items.end(); ++it)
    {
        auto item = qobject_cast<DataGridRowItemPresenter*>(it.value());

        if (item)
        {
            item->setObservableObject(m_observableObject);
        }
    }
}
