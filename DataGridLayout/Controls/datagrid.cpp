#include "datagrid.h"
#include "datagridcolumn.h"
#include "datagridheaderpresenter.h"
#include "datagriditemlayout.h"
#include "datagridrowpresenter.h"
#include "datagridsortfilterproxymodel.h"
#include "filteracceptsroweventargs.h"
#include "observableobjecteventargs.h"

DataGrid::DataGrid(QQuickItem *parent) : QQuickItem(parent)
{
    m_alternativeRowBackground = "#eee";
    m_autogenerateColumns = false;
    m_backgroundEnabled = true;
    m_currentIndex = -1;
    m_defaultHeaderDelegate = NULL;
    m_defaultItemDelegate = NULL;
    m_header = NULL;
    m_headerBackground = "#ddd";
    m_highlightColor = "steelblue";
    m_inactiveHighlightColor = "#e6e6e6";
    m_isReadOnly = false;
    m_itemHeight = 50;
    m_itemLayout = new DataGridItemLayout(this);
    m_layout = NULL;
    m_model = NULL;
    m_scrollBar = NULL;
    m_selectionMode = NoSelection;
    m_sortEnabled = true;
    m_sortFilterProxyModel = new DataGridSortFilterProxyModel(this, this);
    m_updateScrollBar = false;
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(50);
    m_updateTimer->setSingleShot(true);

    connect(m_itemLayout, &DataGridItemLayout::layoutChanged, this, &DataGrid::layoutWidthChanged);
    connect(m_sortFilterProxyModel, &QAbstractItemModel::dataChanged, this, &DataGrid::modelDataChanged);
    connect(m_sortFilterProxyModel, &QAbstractItemModel::layoutChanged, this, &DataGrid::modelLayoutChanged);
    connect(m_sortFilterProxyModel, &QAbstractItemModel::modelReset, this, &DataGrid::modelReset);
    connect(m_sortFilterProxyModel, &QAbstractItemModel::rowsInserted, this, &DataGrid::modelRowsInserted);
    connect(m_sortFilterProxyModel, &QAbstractItemModel::rowsMoved, this, &DataGrid::modelRowsMoved);
    connect(m_sortFilterProxyModel, &QAbstractItemModel::rowsRemoved, this, &DataGrid::modelRowsRemoved);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateVisibleRange()));
    connect(this, &DataGrid::widthChanged, m_itemLayout, &DataGridItemLayout::parentLayoutChanged);

    setAcceptedMouseButtons(Qt::AllButtons);
}

bool DataGrid::autogenerateColumns() const
{
    return m_autogenerateColumns;
}

bool DataGrid::backgroundEnabled() const
{
    return m_backgroundEnabled;
}

bool DataGrid::filterAcceptsRowHandler(int source_row, const QModelIndex &source_parent)
{
    auto eventArgs = new FilterAcceptsRowEventArgs(source_row, source_parent);

    emit filterAcceptsRow(eventArgs);

    auto res = eventArgs->accepted();

    eventArgs->deleteLater();

    return res;
}

bool DataGrid::isReadOnly() const
{
    return m_isReadOnly;
}

bool DataGrid::isRowSelected(int row)
{
    switch (m_selectionMode)
    {
        case DataGrid::MultipleRows:
            return false;
        case DataGrid::NoSelection:
            return false;
        case DataGrid::SingleRow:
            return m_currentIndex == row;
        default:
            return false;
    }
}

bool DataGrid::sortEnabled() const
{
    return m_sortEnabled;
}

DataGrid::SelectionMode DataGrid::selectionMode() const
{
    return m_selectionMode;
}

DataGridColumn *DataGrid::getColumnByIndex(int index)
{
    return index >= 0 && index < m_columns.size() ? m_columns[index] : NULL;
}

DataGridColumn *DataGrid::getColumnByRole(QString role)
{
    for (auto column : m_columns)
    {
        if (column->role() == role)
        {
            return column;
        }
    }
    return Q_NULLPTR;
}

DataGridItemLayout* DataGrid::itemLayout() const
{
    return m_itemLayout;
}

DataGridRowPresenter* DataGrid::popItemFromPool()
{
    return m_itemsPool.size() ? m_itemsPool.dequeue() : NULL;
}

int DataGrid::currentIndex() const
{
    return m_currentIndex;
}

int DataGrid::getColumnIndex(DataGridColumn *column)
{
    return m_columns.indexOf(column);
}

int DataGrid::getFirstDisplayedIndex()
{
    int count = m_items.size(), y = 0;
    auto p = m_scrollBar->mapToItem(m_layout, QPointF(0, 0));

    for (int i = 0; i < count; i++)
    {
        if (y >= p.y())
        {
            return i;
        }

        y += m_items[i];
    }
    return 0;
}

int DataGrid::getLastDisplayedIndex()
{
    int count = m_items.size(), y = 0;
    auto p = m_scrollBar->mapToItem(m_layout, QPointF(0, m_scrollBar->height()));

    for (int i = 0; i < count; i++)
    {
        if (y + m_items[i] > p.y())
        {
            return i - 1;
        }

        y += m_items[i];
    }
    return m_items.size() - 1;
}

int DataGrid::getRowOffset(int row)
{
    int count = m_items.size(), height = 0;

    for (int i = 0; i < row && i < count; i++)
    {
        height += m_items[i];
    }

    return height;
}

int DataGrid::itemHeight() const
{
    return m_itemHeight;
}

int DataGrid::rowCount() const
{
    return m_sortFilterProxyModel->rowCount();
}

QAbstractItemModel* DataGrid::model() const
{
    return m_model;
}

QList<DataGridColumn *> DataGrid::columns() const
{
    return m_columns;
}

QQmlComponent *DataGrid::defaultHeaderDelegate() const
{
    return m_defaultHeaderDelegate;
}

QQmlComponent *DataGrid::defaultItemDelegate() const
{
    return m_defaultItemDelegate;
}

qreal DataGrid::headerHeight() const
{
    return m_header == NULL ? 0 : m_header->height();
}

qreal DataGrid::layoutWidth() const
{
    return m_itemLayout->columnSize(0, INT_MAX);
}

QObject *DataGrid::getObservableObject(int row)
{
    ObservableObjectEventArgs arg;

    arg.setRow(row);

    emit observableObjectRequest(&arg);

    return arg.object();
}

QPair<int, int> DataGrid::displayedRowRange()
{
    int first = 0, last = -1;

    if (m_layout != NULL && m_scrollBar != NULL)
    {
        auto p = m_scrollBar->mapToItem(m_layout, QPointF(0, 0));
        double d = p.y() < 0 ? -p.y() : 0;
        int count = m_items.size(), h = 0, y = 0;

        p.setY(p.y() + d);

        for (; first < count; first++)
        {
            h = m_items[first];

            if (y <= p.y() && y + h > p.y())
            {
                break;
            }

            y += h;
        }

        p = m_scrollBar->mapToItem(m_layout, QPointF(0, m_scrollBar->height()));
        p.setY(p.y() + d);

        if (first < count)
        {
            for (last = first; last < count; last++)
            {
                h = m_items[last];

                if (y + h >= p.y())
                {
                    break;
                }

                y += h;
            }
        }
    }

    return QPair<int, int>(first, last);
}

QSortFilterProxyModel *DataGrid::sortFilterProxyModel() const
{
    return m_sortFilterProxyModel;
}

QString DataGrid::alternativeRowBackground() const
{
    return m_alternativeRowBackground;
}

QString DataGrid::headerBackground() const
{
    return m_headerBackground;
}

QString DataGrid::highlightColor() const
{
    return m_highlightColor;
}

QString DataGrid::inactiveHighlightColor() const
{
    return m_inactiveHighlightColor;
}

QString DataGrid::observablePropertyName() const
{
    return m_observablePropertyName;
}

QStringList DataGrid::skipRoles() const
{
    return m_skipRoles;
}

Qt::CaseSensitivity DataGrid::filterCaseSensitivity() const
{
    return m_sortFilterProxyModel->filterCaseSensitivity();
}

QVariant DataGrid::getUnfilteredValue(int rowIndex, QString role)
{
    if (m_model
        && rowIndex >= 0
        && rowIndex < m_model->rowCount())
    {
        auto roleIndex = m_model->roleNames().key(role.toLocal8Bit());

        return m_model ? m_model->data(m_model->index(rowIndex, 0), roleIndex) : QVariant();
    }
    return QVariant();
}

void DataGrid::alignRow(int row)
{
    if (m_layout != NULL && m_scrollBar != NULL && row >= 0 && row < m_items.size())
    {
        auto contentY = m_scrollBar->property("contentY").toReal();
        auto height = m_items[row];
        auto pos = m_scrollBar->mapFromItem(m_layout, QPointF(0, getRowOffset(row)));

        if (pos.y() < 0)
        {
            m_scrollBar->setProperty("contentY", contentY + pos.y());
        }
        else if (pos.y() + height > m_scrollBar->height())
        {
            m_scrollBar->setProperty("contentY", contentY + pos.y() + height - m_scrollBar->height());
        }
    }
}

void DataGrid::alignRowToBottom(int row)
{
    if (m_layout != NULL && m_scrollBar != NULL && row >= 0 && row < m_items.size())
    {
        auto contentY = m_scrollBar->property("contentY").toReal();
        auto height = m_items[row];
        auto pos = m_scrollBar->mapFromItem(m_layout, QPointF(0, getRowOffset(row)));
        auto y = contentY + pos.y() + height - m_scrollBar->height();

        if (y < 0)
        {
            y = 0;
        }

        m_scrollBar->setProperty("contentY", y);
    }
}

void DataGrid::alignRowToTop(int row)
{
    if (m_layout != NULL && m_scrollBar != NULL && row >= 0 && row < m_items.size())
    {
        auto contentY = m_scrollBar->property("contentY").toReal();
        auto pos = m_scrollBar->mapFromItem(m_layout, QPointF(0, getRowOffset(row)));

        m_scrollBar->setProperty("contentY", contentY + pos.y());
    }
}

void DataGrid::clearAutogeneratedColumns()
{
    for (int i = m_columns.size() - 1; i >= 0; i--)
    {
        auto column = m_columns.at(i);

        if (column->isAutoGenerated())
        {
            m_columns.takeAt(i)->deleteLater();
        }
    }
}

void DataGrid::clearRows()
{
    for (auto item : m_itemsPool)
    {
        disconnect(item, &QQuickItem::heightChanged, NULL, NULL);

        item->setParent(NULL);
        item->deleteLater();
    }

    m_items.clear();
    m_itemsPool.clear();
}

void DataGrid::componentComplete()
{
    QQuickItem::componentComplete();

    for (auto child : children())
    {
        auto column = dynamic_cast<DataGridColumn*>(child);

        if (column)
        {
            column->setDataGrid(this);

            m_columns.push_back(column);
        }
    }

    m_layout = qobject_cast<QQuickItem*>(findChild<QObject*>("__DATAGRIDLAYOUT__"));

    m_header = qobject_cast<DataGridHeaderPresenter*>(findChild<QObject*>("__DATAGRIDHEADER__"));
    if (m_header != NULL)
    {
        m_header->setDataGrid(this);

        connect(m_header, &DataGridHeaderPresenter::heightChanged, this, &DataGrid::headerHeightChanged);
    }

    m_scrollBar = qobject_cast<QQuickItem*>(findChild<QObject*>("__DATAGRIDSCROLLBAR__"));
    if (m_scrollBar != NULL)
    {
        connect(m_scrollBar, SIGNAL(heightChanged()), m_updateTimer, SLOT(start()));
    }

    m_itemLayout->refresh();

    populateRows();
}

void DataGrid::decrementCurrentIndex()
{
    if (m_currentIndex > 0)
    {
        setCurrentIndex(m_currentIndex - 1);
    }
}

void DataGrid::incrementCurrentIndex()
{
    if (m_currentIndex < m_items.size() - 1)
    {
        setCurrentIndex(m_currentIndex + 1);
    }
}

void DataGrid::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::MoveToEndOfDocument))
    {
        setCurrentIndex(m_items.size() - 1);

        alignRow(m_currentIndex);
    }
    else if (event->matches(QKeySequence::MoveToNextLine))
    {
        incrementCurrentIndex();

        alignRow(m_currentIndex);
    }
    else if (event->matches(QKeySequence::MoveToNextPage))
    {
        int index = getLastDisplayedIndex() + 1;

        if (index >= m_items.size() - 1)
        {
            return;
        }

        alignRowToTop(index);

        setCurrentIndex(index = getLastDisplayedIndex(), false);

        alignRowToBottom(index);
    }
    else if (event->matches(QKeySequence::MoveToPreviousLine))
    {
        decrementCurrentIndex();

        alignRow(m_currentIndex);
    }
    else if (event->matches(QKeySequence::MoveToPreviousPage))
    {
        int index = getFirstDisplayedIndex() - 1;

        if (index < 0)
        {
            return;
        }

        alignRowToBottom(index);

        setCurrentIndex(index = getFirstDisplayedIndex(), false);

        alignRowToTop(index);
    }
    else if (event->matches(QKeySequence::MoveToStartOfDocument))
    {
        setCurrentIndex(0);
    }
}

void DataGrid::modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(roles);

    for (int i = topLeft.row(); i <= bottomRight.row(); i++)
    {
        if (m_displayedItems.contains(i))
        {
            m_displayedItems[i]->refresh();
        }
    }
}

void DataGrid::modelLayoutChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint)
{
    Q_UNUSED(hint);
    Q_UNUSED(parents);

    if (m_layout != NULL)
    {
        m_items.clear();

        auto count = m_sortFilterProxyModel->rowCount();

        for (auto i = 0; i < count; i++)
        {
            m_items.push_back(m_itemHeight);
        }

        updateScrollBar();
        updateVisibleRange(true);

        for (auto column : m_columns)
        {
            if (column->sortActive() && column->sortEnabled())
            {
                if (m_sortFilterProxyModel->sortRole() != column->roleIndex())
                {
                    m_sortFilterProxyModel->setSortRole(column->roleIndex());
                    m_sortFilterProxyModel->sort(0, Qt::AscendingOrder);
                }
                break;
            }
        }
    }

    emit rowCountChanged();
}

void DataGrid::modelReset()
{
    clearRows();
    populateAutogeneratedColumns();
    updateScrollBar();
    updateVisibleRange();

    emit rowCountChanged();
}

void DataGrid::modelRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);

    for (int i = first; i <= last; i++)
    {
        m_items.insert(first, m_itemHeight);
    }

    updateScrollBar();

    bool visibleRangeHasToBeUpdated = false;

    for (auto it = m_displayedItems.begin(); it != m_displayedItems.end(); ++it)
    {
        if (it.key() >= first && it.key() <= last)
        {
            visibleRangeHasToBeUpdated = true;

            break;
        }
    }

    if (visibleRangeHasToBeUpdated)
    {
        updateVisibleRange(true);
    }

    emit rowCountChanged();
}

void DataGrid::modelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
{
    Q_UNUSED(destination);
    Q_UNUSED(parent);

    QList<int> moved;

    for (int i = start; i <= end; i++)
    {
        moved.push_back(m_items[start]);

        m_items.removeAt(start);
    }

    int count = moved.size();

    for (int i = 0; i < count; i++)
    {
        m_items.insert(row + i, moved[i]);
    }

    updateScrollBar();

    bool visibleRangeHasToBeUpdated = false;

    for (auto it = m_displayedItems.begin(); it != m_displayedItems.end(); ++it)
    {
        if ((it.key() >= start && it.key() <= end)
            && (it.key() >= row && it.key() < row + count))
        {
            visibleRangeHasToBeUpdated = true;

            break;
        }
    }

    if (visibleRangeHasToBeUpdated)
    {
        updateVisibleRange(true);
    }
}

void DataGrid::modelRowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);

    for (int i = first; i <= last; i++)
    {
        m_items.removeAt(first);
    }

    updateScrollBar();

    bool visibleRangeHasToBeUpdated = false;

    for (auto it = m_displayedItems.begin(); it != m_displayedItems.end(); ++it)
    {
        if (it.key() >= first && it.key() <= last)
        {
            visibleRangeHasToBeUpdated = true;

            break;
        }
    }

    if (visibleRangeHasToBeUpdated)
    {
        updateVisibleRange(true);
    }

    emit rowCountChanged();
}

void DataGrid::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    setFocus(true);
}

void DataGrid::populateAutogeneratedColumns()
{
    if (m_autogenerateColumns && m_model)
    {
        auto roleNames = m_model->roleNames();
        QSet<QString> existingRoles;

        for (int i = m_columns.size() - 1; i >= 0; i--)
        {
            auto column = m_columns.at(i);

            if (column->isAutoGenerated())
            {
                if (roleNames.key(column->role().toString().toUtf8(), 0))
                {
                    existingRoles.insert(column->role().toString());
                }
                else
                {
                    m_columns.takeAt(i)->deleteLater();
                }

                column->setMinWidth(10);
                column->setSizeMode(DataGridColumn::AutoSize);
            }
        }

        auto keys = roleNames.keys();
        DataGridColumn *column = Q_NULLPTR;

        qSort(keys);

        for (auto key : keys)
        {
            auto role = roleNames.value(key);

            if (!existingRoles.contains(role) && !m_skipRoles.contains(role))
            {
                column = new DataGridColumn(this);
                column->setColumn(m_columns.size());
                column->setDataGrid(this);
                column->setIsAutoGenerated(true);
                column->setRole(roleNames.value(key));
                column->setRow(0);
                column->setSizeMode(DataGridColumn::AutoSize);
                column->setSortEnabled(false);

                m_columns.push_back(column);
            }
        }

        if (column)
        {
            column->setMinWidth(150);
            column->setSizeMode(DataGridColumn::RemainingSize);
        }

        if (m_header)
        {
            m_header->createLayout();
        }
        m_itemLayout->refresh();
    }
}

void DataGrid::populateRows()
{
    if (m_layout != NULL)
    {
        auto count = m_sortFilterProxyModel->rowCount();

        for (auto i = 0; i < count; i++)
        {
            m_items.push_back(m_itemHeight);
        }

        updateScrollBar();
        updateVisibleRange();

        emit headerHeightChanged();
    }
}

void DataGrid::pushItemToPool(DataGridRowPresenter* item)
{
    m_itemsPool.append(item);
}

void DataGrid::rowHeightChanged()
{
    auto source = qobject_cast<DataGridRowPresenter*>(sender());

    if (source)
    {
        auto itemRow = source->itemRow();

        if (itemRow >= 0 && itemRow < m_items.size())
        {
            m_items[itemRow] = source->height();
            m_updateScrollBar = true;
            m_updateTimer->start();
        }
    }
}

void DataGrid::selectRow(int row, QMouseEvent *event)
{
    Q_UNUSED(event);

    switch (m_selectionMode)
    {
        case SingleRow:
            setCurrentIndex(row);
            break;
        case MultipleRows:
            break;
    }
}

void DataGrid::setAlternativeRowBackground(QString alternativeRowBackground)
{
    if (m_alternativeRowBackground == alternativeRowBackground)
        return;

    m_alternativeRowBackground = alternativeRowBackground;
    emit alternativeRowBackgroundChanged();
}

void DataGrid::setAutogenerateColumns(bool autogenerateColumns)
{
    if (m_autogenerateColumns == autogenerateColumns)
        return;

    m_autogenerateColumns = autogenerateColumns;
    emit autogenerateColumnsChanged(m_autogenerateColumns);

    if (!m_autogenerateColumns)
    {
        clearAutogeneratedColumns();
    }
    else
    {
        populateAutogeneratedColumns();
    }
}

void DataGrid::setBackgroundEnabled(bool backgroundEnabled)
{
    if (m_backgroundEnabled == backgroundEnabled)
        return;

    m_backgroundEnabled = backgroundEnabled;
    emit backgroundEnabledChanged(m_backgroundEnabled);
}

void DataGrid::setCurrentIndex(int currentIndex, bool align)
{
    if (m_currentIndex == currentIndex)
        return;

    m_currentIndex = currentIndex;
    emit currentIndexChanged(m_currentIndex);
    emit selectionChanged();

    if (align)
    {
        alignRow(m_currentIndex);
    }
}

void DataGrid::setDefaultHeaderDelegate(QQmlComponent *defaultHeaderDelegate)
{
    if (m_defaultHeaderDelegate == defaultHeaderDelegate)
        return;

    m_defaultHeaderDelegate = defaultHeaderDelegate;
    emit defaultHeaderDelegateChanged(m_defaultHeaderDelegate);
}

void DataGrid::setDefaultItemDelegate(QQmlComponent *defaultItemDelegate)
{
    if (m_defaultItemDelegate == defaultItemDelegate)
        return;

    m_defaultItemDelegate = defaultItemDelegate;
    emit defaultItemDelegateChanged(m_defaultItemDelegate);
}

void DataGrid::setFilterCaseSensitivity(Qt::CaseSensitivity filterCaseSensitivity)
{
    m_sortFilterProxyModel->setFilterCaseSensitivity(filterCaseSensitivity);
}

void DataGrid::setHeaderBackground(QString headerBackground)
{
    if (m_headerBackground == headerBackground)
        return;

    m_headerBackground = headerBackground;
    emit headerBackgroundChanged();
}

void DataGrid::setHighlightColor(QString highlightColor)
{
    if (m_highlightColor == highlightColor)
        return;

    m_highlightColor = highlightColor;
    emit highlightColorChanged();
}

void DataGrid::setInactiveHighlightColor(QString inactiveHighlightColor)
{
    if (m_inactiveHighlightColor == inactiveHighlightColor)
        return;

    m_inactiveHighlightColor = inactiveHighlightColor;
    emit inactiveHighlightColorChanged();
}

void DataGrid::setIsReadOnly(bool isReadOnly)
{
    if (m_isReadOnly == isReadOnly)
        return;

    m_isReadOnly = isReadOnly;
    emit isReadOnlyChanged(m_isReadOnly);
}

void DataGrid::setItemHeight(int itemHeight)
{
    if (itemHeight < 0)
    {
        itemHeight = 0;
    }

    if (m_itemHeight == itemHeight)
        return;

    m_itemHeight = itemHeight;
    emit itemHeightChanged(m_itemHeight);
}

void DataGrid::setModel(QAbstractItemModel *model)
{
    if (m_model == model)
        return;

    clearRows();

    m_model = model;
    m_sortFilterProxyModel->setSourceModel(m_model);
    emit modelChanged(m_model);
    emit rowCountChanged();

    populateRows();
}

void DataGrid::setObservablePropertyName(QString observablePropertyName)
{
    if (m_observablePropertyName == observablePropertyName)
        return;

    m_observablePropertyName = observablePropertyName;
    emit observablePropertyNameChanged(m_observablePropertyName);
}

void DataGrid::setSelectionMode(DataGrid::SelectionMode selectionMode)
{
    if (m_selectionMode == selectionMode)
        return;

    m_selectionMode = selectionMode;
    emit selectionModeChanged(m_selectionMode);
}

void DataGrid::setSkipRoles(QStringList skipRoles)
{
    if (m_skipRoles == skipRoles)
        return;

    m_skipRoles = skipRoles;
    emit skipRolesChanged(m_skipRoles);
}

void DataGrid::setSortEnabled(bool sortEnabled)
{
    if (m_sortEnabled == sortEnabled)
        return;

    m_sortEnabled = sortEnabled;
    emit sortEnabledChanged(m_sortEnabled);
}

void DataGrid::updateScrollBar()
{
    if (m_scrollBar != NULL)
    {
        int count = m_items.size(), height = 0;

        for (auto i = 0; i < count; i++)
        {
            height += m_items[i];
        }

        m_scrollBar->setProperty("contentHeight", height);
    }

    m_updateScrollBar = false;
}

void DataGrid::updateVisibleRange(bool refreshVisible)
{
    auto count = m_items.size();
    auto displayedItems = m_displayedItems;
    auto range = displayedRowRange();

    for (auto it = m_displayedItems.constBegin(); it != m_displayedItems.constEnd(); ++it)
    {
        if (it.key() < range.first || it.key() > range.second)
        {
            it.value()->setParentItem(NULL);

            displayedItems.remove(it.key());

            pushItemToPool(it.value());
        }
    }

    m_displayedItems = displayedItems;

    auto totalHeightChanged = false;
    auto y = getRowOffset(range.first);
    QQmlComponent component(qmlEngine(this), "qrc:/DataGridRow.qml");

    for (auto i = range.first; i <= range.second && i < count; i++)
    {
        if (!m_displayedItems.contains(i))
        {
            auto item = popItemFromPool();

            if (item == NULL)
            {
                item = qobject_cast<DataGridRowPresenter*>(component.create());

                item->setItemRow(i);
                item->setDataGrid(this);
                item->setParent(m_layout);

                connect(item, &DataGridRowPresenter::heightChanged, this, &DataGrid::rowHeightChanged);
            }
            else
            {
                if (item->itemRow() == i)
                {
                    item->refresh();
                }
                else
                {
                    item->setItemRow(i);
                }
            }

            item->setParentItem(m_layout);
            item->setY(y);

            if (m_items[i] != item->height())
            {
                totalHeightChanged = true;

                m_items[i] = item->height();
            }

            m_displayedItems[i] = item;
        }
        else
        {
            if (m_items[i] != m_displayedItems[i]->height())
            {
                totalHeightChanged = true;

                m_items[i] = m_displayedItems[i]->height();
            }

            m_displayedItems[i]->setY(y);

            if (refreshVisible)
            {
                m_displayedItems[i]->refresh();
            }
        }

        y += m_items[i];
    }

    if (m_updateScrollBar || totalHeightChanged)
    {
        updateScrollBar();
    }
}
