#ifndef DATAGRIDHEADERPRESENTER_H
#define DATAGRIDHEADERPRESENTER_H

#include <QObject>
#include <QQuickItem>

class DataGrid;
class DataGridColumn;
class DataGridHeaderPresenter : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(DataGrid* dataGrid READ dataGrid WRITE setDataGrid NOTIFY dataGridChanged)
public:
    DataGridHeaderPresenter(QQuickItem *parent = NULL);
    DataGrid* dataGrid() const;
    void arrange();

private:
    void createLayout();

private slots:
    void itemWidthChanged();
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
};

#endif // DATAGRIDHEADERPRESENTER_H
