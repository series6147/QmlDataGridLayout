#ifndef DATAGRIDHEADERITEMPRESENTER_H
#define DATAGRIDHEADERITEMPRESENTER_H

#include <QAbstractItemModel>
#include <QObject>
#include <QPainter>
#include <QQuickPaintedItem>
#include <QSortFilterProxyModel>

class DataGrid;
class DataGridColumn;
class DataGridHeaderItemPresenter : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool itemVisible READ itemVisible WRITE setItemVisible NOTIFY itemVisibleChanged)
    Q_PROPERTY(bool itemVisible READ itemVisible WRITE setItemVisible NOTIFY itemVisibleChanged)
    Q_PROPERTY(DataGrid* dataGrid READ dataGrid WRITE setDataGrid NOTIFY dataGridChanged)
    Q_PROPERTY(DataGridColumn* column READ column WRITE setColumn NOTIFY columnChanged)
    Q_PROPERTY(int itemRow READ itemRow WRITE setItemRow NOTIFY itemRowChanged)
    Q_PROPERTY(int itemRowSpan READ itemRowSpan WRITE setItemRowSpan NOTIFY itemRowSpanChanged)
    Q_PROPERTY(int itemWidth READ itemWidth WRITE setItemWidth NOTIFY itemWidthChanged)
    Q_PROPERTY(int itemX READ itemX WRITE setItemX NOTIFY itemXChanged)
    Q_PROPERTY(QSortFilterProxyModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString color READ color NOTIFY colorChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
    Q_PROPERTY(QVariant modelData READ modelData WRITE setModelData NOTIFY modelDataChanged)
public:
    DataGridHeaderItemPresenter(QQuickItem *parent = Q_NULLPTR);
    bool itemVisible() const;
    DataGrid* dataGrid() const;
    DataGridColumn* column() const;
    int itemRow() const;
    int itemRowSpan() const;
    int itemWidth() const;
    int itemX() const;
    QSortFilterProxyModel* model() const;
    QString color() const;
    Qt::SortOrder sortOrder() const;
    QVariant modelData() const;

signals:
    void colorChanged();
    void columnChanged(DataGridColumn* column);
    void dataGridChanged(DataGrid* dataGrid);
    void itemRowChanged(int itemRow);
    void itemRowSpanChanged(int itemRowSpan);
    void itemVisibleChanged(bool itemVisible);
    void itemWidthChanged(int itemWidth);
    void itemXChanged(int itemX);
    void modelChanged(QSortFilterProxyModel* model);
    void modelDataChanged();
    void sortOrderChanged(Qt::SortOrder sortOrder);

public slots:
    void releseResize();
    void setColumn(DataGridColumn* column);
    void setDataGrid(DataGrid* dataGrid);
    void setItemRow(int itemRow);
    void setItemRowSpan(int itemRowSpan);
    void setItemVisible(bool itemVisible);
    void setItemWidth(int itemWidth);
    void setItemX(int itemX);
    void setModel(QSortFilterProxyModel* model);
    void setModelData(QVariant modelData);
    void setSortOrder(Qt::SortOrder sortOrder);
    void sortColumn();
    void startResize();

private:
    bool m_itemVisible;
    bool m_resizeStarted;
    DataGrid* m_dataGrid;
    DataGridColumn* m_column;
    int m_itemRow;
    int m_itemRowSpan;
    int m_itemWidth;
    int m_itemX;
    int m_originX;
    QSortFilterProxyModel *m_model;
    QQuickItem *m_resizeArea;
    qreal m_originWidth;
    QString m_color;
    Qt::SortOrder m_sortOrder;

    // QQmlParserStatus interface
public:
    void componentComplete() override;

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter) override;

    // QQuickItem interface
protected:
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;
};

#endif // DATAGRIDHEADERITEMPRESENTER_H
