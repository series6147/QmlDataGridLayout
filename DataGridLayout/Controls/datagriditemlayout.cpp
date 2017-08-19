#include "datagrid.h"
#include "datagridcolumn.h"
#include "datagriditemlayout.h"

DataGridItemLayout::DataGridItemLayout(DataGrid *dataGrid, QObject *parent) : QObject(parent)
{
    m_dataGrid = dataGrid;

    connect(m_dataGrid, &DataGrid::widthChanged, this, &DataGridItemLayout::parentLayoutChanged);
}

DataGridItemLayout::DataGridItemLayout(QObject *parent) : QObject(parent)
{
}

qreal DataGridItemLayout::columnOffset(int column)
{
    qreal offset = 0;

    for (int i = 0; i < qMin(column, m_columnsSize.size()); i++)
    {
        offset += m_columnsSize[i];
    }
    return offset;
}

qreal DataGridItemLayout::columnSize(int column, int columnSpan)
{
    qreal size = 0;

    for (int i = column >= 0 ? column : 0; i < qMin(column + columnSpan, m_columnsSize.size()); i++)
    {
        size += m_columnsSize[i];
    }
    return size;
}

qreal DataGridItemLayout::getColumnSizeFromSet(DataGridColumn *column, const QList<qreal> &columnsSize)
{
    int res = 0;

    for (auto col = column->column(); col < column->column() + column->columnSpan(); col++)
    {
        res += columnsSize[col];
    }
    return res;
}

QList<DataGridColumn *> DataGridItemLayout::getIntersectedColumns(int col)
{
    QList<DataGridColumn *> res;

    for (auto column : m_dataGrid->columns())
    {
        if (column->column() <= col && column->column() + column->columnSpan() - 1 >= col)
        {
            res.push_back(column);
        }
    }
    return res;
}

QList<DataGridColumn *> DataGridItemLayout::getIntersectedColumns(int col, QSet<int> justSizeMode, QRegExp *rexp)
{
    QList<DataGridColumn *> res;

    for (auto column : m_dataGrid->columns())
    {
        if (column->column() <= col
            && column->column() + column->columnSpan() - 1 >= col)
        {
            if (justSizeMode.contains(column->sizeMode())
                && (rexp == NULL || rexp->exactMatch(column->width().toString())))
            {
                res.push_back(column);
            }
            else
            {
                return QList<DataGridColumn *>();
            }
        }
    }
    return res;
}

void DataGridItemLayout::arrange()
{
    qreal layoutWidth = 0;
    int columnCount = m_columnsSize.size();
    QRegExp rxStar("^[0-9]+\\*$");

    // maxmin fixed columns

    for (auto col = 0; col < columnCount; col++)
    {
        qreal max = 0;

        for (auto column : getIntersectedColumns(col))
        {
            switch (column->sizeMode())
            {
                case DataGridColumn::AutoSize:
                    max = qMax(max, qMax((qreal)column->minWidth(), column->width().toReal()) / (qreal)column->columnSpan());
                    break;
                case DataGridColumn::RemainingSize:
                    max = qMax(max, column->minWidth() / (qreal)column->columnSpan());
                    break;
                case DataGridColumn::FixedSize:
                    if (rxStar.exactMatch(column->width().toString()))
                    {
                        max = qMax(max, column->minWidth() / (qreal)column->columnSpan());
                    }
                    else
                    {
                        max = qMax(max, qMax((qreal)column->minWidth(), column->width().toReal()) / (qreal)column->columnSpan());
                    }
                    break;
            }
        }

        m_columnsSize[col] = max;
    }
    for (auto col = 0; col < columnCount; col++)
    {
        qreal min = INT_MAX;

        for (auto column : getIntersectedColumns(col))
        {
            int calculatedSize = getColumnSizeFromSet(column, m_columnsSize);

            switch (column->sizeMode())
            {
                case DataGridColumn::AutoSize:
                case DataGridColumn::RemainingSize:
                    if (calculatedSize > column->minWidth())
                    {
                        min = qMin(min, (qreal)(calculatedSize - column->minWidth()));
                    }
                    break;
                case DataGridColumn::FixedSize:
                    if (!rxStar.exactMatch(column->width().toString())
                        && calculatedSize > qMax((qreal)column->minWidth(), column->width().toReal()))
                    {
                        min = qMin(min, (qreal)(calculatedSize - qMax((qreal)column->minWidth(), column->width().toReal())));
                    }
                    break;
            }
        }

        if (min != INT_MAX)
        {
            m_columnsSize[col] = m_columnsSize[col] - min;
        }
    }
    for (auto col = 0; col < columnCount; col++)
    {
        qreal min = INT_MAX;

        for (auto column : getIntersectedColumns(col))
        {
            int calculatedSize = getColumnSizeFromSet(column, m_columnsSize);

            switch (column->sizeMode())
            {
                case DataGridColumn::AutoSize:
                case DataGridColumn::RemainingSize:
                    if (calculatedSize < column->minWidth())
                    {
                        min = qMin(min, (qreal)(column->minWidth() - calculatedSize));
                    }
                    break;
                case DataGridColumn::FixedSize:
                    if (!rxStar.exactMatch(column->width().toString())
                        && calculatedSize < qMax((qreal)column->minWidth(), column->width().toReal()))
                    {
                        min = qMin(min, (qreal)(qMax((qreal)column->minWidth(), column->width().toReal()) - calculatedSize));
                    }
                    break;
            }
        }

        if (min != INT_MAX)
        {
            m_columnsSize[col] = m_columnsSize[col] + min;
        }
    }
    for (auto col = 0; col < columnCount; col++)
    {
        layoutWidth += m_columnsSize[col];
    }

    layoutWidth = qMax(layoutWidth, m_dataGrid->width());

    // alignment star columns

    qreal availableWidth = layoutWidth, totalCoeff = 0;
    QList<QVariant> columnsCoeff;

    for (auto col = 0; col < columnCount; col++)
    {
        qreal max = -1;

        for (auto column : getIntersectedColumns(col, { DataGridColumn::FixedSize }, &rxStar))
        {
            availableWidth -= m_columnsSize[col];

            max = qMax(max, column->width().toString().remove("*").toFloat() / (qreal)column->columnSpan());
        }

        columnsCoeff.push_back(max < 0 ? QVariant() : max);
    }
    for (auto col = 0; col < columnCount; col++)
    {
        if (columnsCoeff[col].isValid())
        {
            totalCoeff += columnsCoeff[col].toReal();
        }
    }
    for (auto col = 0; col < columnCount; col++)
    {
        if (columnsCoeff[col].isValid())
        {
            m_columnsSize[col] = qMax(m_columnsSize[col], availableWidth * columnsCoeff[col].toReal() / totalCoeff);

            availableWidth -= m_columnsSize[col];
            totalCoeff -= columnsCoeff[col].toReal();
        }
    }

    // alignment remaining columns

    int count = 0;
    QMap<qreal, QList<int>> remainSizeColumns;

    availableWidth = layoutWidth;

    for (auto col = 0; col < columnCount; col++)
    {
        if (getIntersectedColumns(col, { DataGridColumn::AutoSize, DataGridColumn::FixedSize }).size() > 0)
        {
            availableWidth -= m_columnsSize[col];
        }
    }
    for (auto col = 0; col < columnCount; col++)
    {
        if (getIntersectedColumns(col, { DataGridColumn::AutoSize, DataGridColumn::FixedSize }).size() == 0)
        {
            count++;
            remainSizeColumns[m_columnsSize[col]].push_back(col);
        }
    }

    QList<qreal> keys = remainSizeColumns.keys();

    qSort(keys);

    for (auto key : keys)
    {
        qreal remainWidth = availableWidth / (qreal)count;

        for (auto col : remainSizeColumns[key])
        {
            if (key <= remainWidth)
            {
                m_columnsSize[col] = remainWidth;
            }

            availableWidth -= m_columnsSize[col];
            count--;
        }
    }

    emit layoutChanged();
}

void DataGridItemLayout::parentLayoutChanged()
{
    arrange();
}

void DataGridItemLayout::refresh()
{
    int columnCount = 0;

    m_columnsSize.clear();

    for (auto column : m_dataGrid->columns())
    {
        columnCount = qMax(columnCount, column->column() + column->columnSpan());
    }

    for (auto i = 0; i < columnCount; i++)
    {
        m_columnsSize.push_back(0);
    }

    arrange();
}

void DataGridItemLayout::resizeColumn(DataGridColumn *column, qreal size)
{
    if (column->sizeMode() == DataGridColumn::FixedSize
        && size >= column->minWidth())
    {
        bool ok;

        if (column->width().toReal(&ok) && ok)
        {
            column->setWidth(size);

            arrange();
        }
    }
}

void DataGridItemLayout::setColumnSize(DataGridColumn *column, qreal size)
{
    if (column->sizeMode() == DataGridColumn::AutoSize)
    {
        column->setWidth(size);

        arrange();
    }
}
