#ifndef DATAGRIDHEADERITEMPRESENTER_H
#define DATAGRIDHEADERITEMPRESENTER_H

#include <QAbstractItemModel>
#include <QObject>
#include <QPainter>
#include <QQuickItemGrabResult>
#include <QQuickPaintedItem>
#include <QSortFilterProxyModel>

class DataGrid;
class DataGridColumn;
class DataGridHeaderItemPresenter : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool highlighted READ highlighted WRITE setHighlighted)
    Q_PROPERTY(bool itemVisible READ itemVisible WRITE setItemVisible NOTIFY itemVisibleChanged)
    Q_PROPERTY(DataGrid* dataGrid READ dataGrid WRITE setDataGrid NOTIFY dataGridChanged)
    Q_PROPERTY(DataGridColumn* column READ column WRITE setColumn NOTIFY columnChanged)
    Q_PROPERTY(int itemRow READ itemRow WRITE setItemRow NOTIFY itemRowChanged)
    Q_PROPERTY(int itemRowSpan READ itemRowSpan WRITE setItemRowSpan NOTIFY itemRowSpanChanged)
    Q_PROPERTY(int itemX READ itemX WRITE setItemX NOTIFY itemXChanged)
    Q_PROPERTY(qreal itemWidth READ itemWidth WRITE setItemWidth NOTIFY itemWidthChanged)
    Q_PROPERTY(QSortFilterProxyModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString color READ color NOTIFY colorChanged)
    Q_PROPERTY(QVariant modelData READ modelData WRITE setModelData NOTIFY modelDataChanged)
public:
    DataGridHeaderItemPresenter(QQuickItem *parent = Q_NULLPTR);
    bool highlighted() const;
    bool itemVisible() const;
    DataGrid* dataGrid() const;
    DataGridColumn* column() const;
    int itemRow() const;
    int itemRowSpan() const;
    int itemX() const;
    qreal itemWidth() const;
    QSortFilterProxyModel* model() const;
    QString color() const;
    QVariant modelData() const;

private slots:
    void dataGridDestroyed();

signals:
    void colorChanged();
    void columnChanged(DataGridColumn* column);
    void dataGridChanged(DataGrid* dataGrid);
    void itemRowChanged(int itemRow);
    void itemRowSpanChanged(int itemRowSpan);
    void itemVisibleChanged(bool itemVisible);
    void itemWidthChanged(qreal itemWidth);
    void itemXChanged(int itemX);
    void modelChanged(QSortFilterProxyModel* model);
    void modelDataChanged();

public slots:
    void contentWidthChanged(qreal width);
    void releseResize();
    void setColumn(DataGridColumn* column);
    void setDataGrid(DataGrid* dataGrid);
    void setHighlighted(bool highlighted);
    void setItemRow(int itemRow);
    void setItemRowSpan(int itemRowSpan);
    void setItemVisible(bool itemVisible);
    void setItemWidth(qreal itemWidth);
    void setItemX(int itemX);
    void setModel(QSortFilterProxyModel* model);
    void setModelData(QVariant modelData);
    void sortColumn();
    void startResize();
    void touchMoved(qreal x, qreal y);
    void touchReleased();
    void touchStarted(qreal x, qreal y);

private:
    bool m_highlighted;
    bool m_itemVisible;
    bool m_resizeStarted;
    DataGrid* m_dataGrid;
    DataGridColumn* m_column;
    int m_itemRow;
    int m_itemRowSpan;
    int m_itemX;
    int m_originX;
    QSortFilterProxyModel *m_model;
    QQuickItem *m_resizeArea;
    qreal m_itemWidth;
    qreal m_originWidth;
    QString m_color;

    // QQmlParserStatus interface
public:
    void componentComplete() override;

    // QQuickItem interface
protected:
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;
};

#endif // DATAGRIDHEADERITEMPRESENTER_H
