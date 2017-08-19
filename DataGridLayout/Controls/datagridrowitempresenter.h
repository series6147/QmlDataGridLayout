#ifndef DATAGRIDROWITEMPRESENTER_H
#define DATAGRIDROWITEMPRESENTER_H

#include <QAbstractItemModel>
#include <QObject>
#include <QQuickItem>

class DataGrid;
class DataGridColumn;
class DataGridRowItemPresenter : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool isSelected READ isSelected NOTIFY isSelectedChanged)
    Q_PROPERTY(bool itemVisible READ itemVisible WRITE setItemVisible NOTIFY itemVisibleChanged)
    Q_PROPERTY(DataGrid* dataGrid READ dataGrid WRITE setDataGrid NOTIFY dataGridChanged)
    Q_PROPERTY(DataGridColumn* column READ column WRITE setColumn NOTIFY columnChanged)
    Q_PROPERTY(int itemRow READ itemRow WRITE setItemRow NOTIFY itemRowChanged)
    Q_PROPERTY(int itemRowSpan READ itemRowSpan WRITE setItemRowSpan NOTIFY itemRowSpanChanged)
    Q_PROPERTY(int itemWidth READ itemWidth WRITE setItemWidth NOTIFY itemWidthChanged)
    Q_PROPERTY(int itemX READ itemX WRITE setItemX NOTIFY itemXChanged)
    Q_PROPERTY(int rowIndex READ rowIndex WRITE setRowIndex NOTIFY rowIndexChanged)
    Q_PROPERTY(QAbstractItemModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QObject* observableObject READ observableObject WRITE setObservableObject NOTIFY observableObjectChanged)
    Q_PROPERTY(QVariant modelData READ modelData WRITE setModelData NOTIFY modelDataChanged)
public:
    DataGridRowItemPresenter(QQuickItem *parent = Q_NULLPTR);
    bool itemVisible() const;
    bool isSelected() const;
    DataGrid* dataGrid() const;
    DataGridColumn* column() const;
    int itemRow() const;
    int itemRowSpan() const;
    int itemWidth() const;
    int itemX() const;
    int rowIndex() const;
    QAbstractItemModel* model() const;
    QObject* observableObject() const;
    QVariant modelData() const;

public slots:
    void sendEvent(QString eventName, QVariant value);
    void setColumn(DataGridColumn* column);
    void setDataGrid(DataGrid* dataGrid);
    void setItemRow(int itemRow);
    void setItemRowSpan(int itemRowSpan);
    void setItemVisible(bool itemVisible);
    void setItemWidth(int itemWidth);
    void setItemX(int itemX);
    void setModel(QAbstractItemModel* model);
    void setModelData(QVariant data);
    void setObservableObject(QObject* observableObject);
    void setRowIndex(int rowIndex);

signals:
    void columnChanged(DataGridColumn* column);
    void dataGridChanged(DataGrid* dataGrid);
    void isSelectedChanged();
    void itemRowChanged(int itemRow);
    void itemRowSpanChanged(int itemRowSpan);
    void itemVisibleChanged(bool itemVisible);
    void itemWidthChanged(int itemWidth);
    void itemXChanged(int itemX);
    void modelChanged(QAbstractItemModel* model);
    void modelDataChanged();
    void observableObjectChanged(QObject* observableObject);
    void rowIndexChanged(int rowIndex);

private:
    bool m_itemVisible;
    DataGrid* m_dataGrid;
    DataGridColumn* m_column;
    int m_itemRow;
    int m_itemRowSpan;
    int m_itemWidth;
    int m_itemX;
    int m_rowIndex;
    QAbstractItemModel* m_model;
    QObject* m_observableObject;
};

#endif // DATAGRIDROWITEMPRESENTER_H
