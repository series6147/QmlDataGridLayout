#ifndef DATAGRIDHEADERPRESENTER_H
#define DATAGRIDHEADERPRESENTER_H

#include <QObject>
#include <QQuickPaintedItem>

class DataGrid;
class DataGridColumn;
class DataGridHeaderPresenter : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(DataGrid* dataGrid READ dataGrid WRITE setDataGrid NOTIFY dataGridChanged)
public:
    DataGridHeaderPresenter(QQuickItem *parent = NULL);
    DataGrid* dataGrid() const;
    void arrange();
    void createLayout();

private slots:
    void layoutChanged();

public slots:
    void setDataGrid(DataGrid* dataGrid);

signals:
    void dataGridChanged(DataGrid* dataGrid);

private:
    DataGrid* m_dataGrid;
    QMap<DataGridColumn*, QQuickItem*> m_items;
    QQuickItem* m_itemsLayout;

    // QQmlParserStatus interface
public:
    void componentComplete() override;

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter) override;
};

#endif // DATAGRIDHEADERPRESENTER_H
