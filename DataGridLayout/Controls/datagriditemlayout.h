#ifndef DATAGRIDITEMLAYOUT_H
#define DATAGRIDITEMLAYOUT_H

#include <QObject>
#include <QTimer>
#include <QVariant>

class DataGrid;
class DataGridColumn;
class DataGridItemLayout : public QObject
{
    Q_OBJECT
public:
    explicit DataGridItemLayout(DataGrid *dataGrid, QObject *parent = nullptr);
    explicit DataGridItemLayout(QObject *parent = nullptr);
    qreal columnOffset(int column);
    qreal columnSize(int column, int columnSpan);
    void refresh();

private:
    qreal getColumnSizeFromSet(DataGridColumn *column, const QList<qreal> &columnsSize);
    QList<DataGridColumn*> getIntersectedColumns(int col);
    QList<DataGridColumn*> getIntersectedColumns(int col, QSet<int> justSizeMode, QRegExp *rexp = NULL);

public slots:
    void arrange();
    void parentLayoutChanged();
    void resizeColumn(DataGridColumn *column, qreal size);
    void setColumnSize(DataGridColumn *column, qreal size);

signals:
    void layoutChanged();

private:
    DataGrid *m_dataGrid;
    QList<qreal> m_columnsSize;
};

#endif // DATAGRIDITEMLAYOUT_H
