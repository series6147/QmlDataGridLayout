#ifndef DATAGRIDROWPRESENTER_H
#define DATAGRIDROWPRESENTER_H

#include <QObject>
#include <QPainter>
#include <QQmlContext>
#include <QQuickPaintedItem>

class DataGrid;
class DataGridColumn;
class DataGridRowPresenter : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(DataGrid* dataGrid READ dataGrid WRITE setDataGrid NOTIFY dataGridChanged)
    Q_PROPERTY(int itemRow READ itemRow WRITE setItemRow NOTIFY itemRowChanged)
    Q_PROPERTY(QObject* observableObject READ observableObject WRITE setObservableObject NOTIFY observableObjectChanged)
public:
    DataGridRowPresenter(QQuickItem *parent = Q_NULLPTR);
    DataGrid* dataGrid() const;
    int itemRow() const;
    QObject* observableObject() const;
    void arrange();
    void refresh();

private:
    void createLayout();

private slots:
    void layoutChanged();
    void parentFocusChanged();

public slots:
    void setDataGrid(DataGrid* dataGrid);
    void setItemRow(int itemRow);
    void setObservableObject(QObject* observableObject);

signals:
    void dataGridChanged(DataGrid* dataGrid);
    void itemRowChanged(int itemRow);
    void observableObjectChanged(QObject* observableObject);

private:
    DataGrid* m_dataGrid;
    int m_itemRow;
    QMap<DataGridColumn*, QQuickItem*> m_items;
    QObject* m_observableObject;
    QQuickItem* m_itemsLayout;

    // QQmlParserStatus interface
public:
    void componentComplete() override;

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter) override;

protected:
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // DATAGRIDROWPRESENTER_H
