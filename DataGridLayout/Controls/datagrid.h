#ifndef DATAGRID_H
#define DATAGRID_H

#include <QAbstractItemModel>
#include <QMutex>
#include <QQmlEngine>
#include <QQueue>
#include <QQuickItem>
#include <QQuickWindow>
#include <QSortFilterProxyModel>
#include <QTimer>

class DataGridColumn;
class DataGridHeaderPresenter;
class DataGridItemLayout;
class DataGridRowPresenter;
class ObservableObjectEventArgs;
class DataGrid : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool sortEnabled READ sortEnabled WRITE setSortEnabled NOTIFY sortEnabledChanged)
    Q_PROPERTY(DataGridItemLayout* itemLayout READ itemLayout)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(QAbstractItemModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString alternativeRowBackground READ alternativeRowBackground WRITE setAlternativeRowBackground NOTIFY alternativeRowBackgroundChanged)
    Q_PROPERTY(QString inactiveHighlightColor READ inactiveHighlightColor WRITE setInactiveHighlightColor NOTIFY inactiveHighlightColorChanged)
    Q_PROPERTY(QString headerBackground READ headerBackground WRITE setHeaderBackground NOTIFY headerBackgroundChanged)
    Q_PROPERTY(QString highlightColor READ highlightColor WRITE setHighlightColor NOTIFY highlightColorChanged)
    Q_PROPERTY(QString observablePropertyName READ observablePropertyName WRITE setObservablePropertyName NOTIFY observablePropertyNameChanged)
    Q_PROPERTY(SelectionMode selectionMode READ selectionMode WRITE setSelectionMode NOTIFY selectionModeChanged)
public:
    enum SelectionMode {
        NoSelection = 0,
        SingleRow = 1,
        MultipleRows = 2
    };

    Q_ENUM(SelectionMode)

    explicit DataGrid(QQuickItem *parent = Q_NULLPTR);
    bool sortEnabled() const;
    DataGridColumn* getColumnByIndex(int index);
    DataGridItemLayout* itemLayout() const;
    int currentIndex() const;
    int getColumnIndex(DataGridColumn* column);
    int itemHeight() const;
    QAbstractItemModel* model() const;
    QList<DataGridColumn*> columns() const;
    QSortFilterProxyModel* sortFilterProxyModel() const;
    QString alternativeRowBackground() const;
    QString headerBackground() const;
    QString highlightColor() const;
    QString inactiveHighlightColor() const;
    QString observablePropertyName() const;
    SelectionMode selectionMode() const;
    void decrementCurrentIndex();
    void incrementCurrentIndex();
    void selectRow(int row, QMouseEvent *event = NULL);

private:
    DataGridRowPresenter* popItemFromPool();
    int getFirstDisplayedIndex();
    int getLastDisplayedIndex();
    int getRowOffset(int row);
    QPair<int, int> displayedRowRange();
    void alignRow(int row);
    void alignRowToBottom(int row);
    void alignRowToTop(int row);
    void clearRows();
    void populateRows();
    void pushItemToPool(DataGridRowPresenter* item);
    void updateScrollBar();

private slots:
    void modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
    void modelLayoutChanged(const QList<QPersistentModelIndex> &parents = QList<QPersistentModelIndex>(), QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);
    void modelReset();
    void modelRowsInserted(const QModelIndex &parent, int first, int last);
    void modelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    void modelRowsRemoved(const QModelIndex &parent, int first, int last);
    void rowHeightChanged();

public slots:
    bool isRowSelected(int row);
    virtual QObject* getObservableObject(int row);
    void setAlternativeRowBackground(QString alternativeRowBackground);
    void setCurrentIndex(int currentIndex, bool align = true);
    void setHeaderBackground(QString headerBackground);
    void setHighlightColor(QString highlightColor);
    void setInactiveHighlightColor(QString inactiveHighlightColor);
    void setItemHeight(int itemHeight);
    void setModel(QAbstractItemModel *model);
    void setObservablePropertyName(QString observablePropertyName);
    void setSelectionMode(SelectionMode selectionMode);
    void setSortEnabled(bool sortEnabled);
    void updateVisibleRange(bool refreshVisible = false);

signals:
    void alternativeRowBackgroundChanged();
    void columnsChanged(QList<DataGridColumn*> columns);
    void currentIndexChanged(int currentIndex);
    void headerBackgroundChanged();
    void highlightColorChanged();
    void inactiveHighlightColorChanged();
    void itemHeightChanged(int itemHeight);
    void modelChanged(QAbstractItemModel *model);
    void observableObjectRequest(ObservableObjectEventArgs *arg);
    void observablePropertyNameChanged(QString observablePropertyName);
    void selectionChanged();
    void selectionModeChanged(SelectionMode selectionMode);
    void sortEnabledChanged(bool sortEnabled);
    void userEvent(QString eventName, QVariant value);

private:
    bool m_sortEnabled;
    DataGridHeaderPresenter *m_header;
    DataGridItemLayout* m_itemLayout;
    int m_currentIndex;
    int m_itemHeight;
    QAbstractItemModel* m_model;
    QList<DataGridColumn*> m_columns;
    QList<int> m_items;
    QMap<int, DataGridRowPresenter*> m_displayedItems;
    QQueue<DataGridRowPresenter*> m_itemsPool;
    QQuickItem* m_layout;
    QQuickItem* m_scrollBar;
    QSortFilterProxyModel* m_sortFilterProxyModel;
    QString m_alternativeRowBackground;
    QString m_headerBackground;
    QString m_highlightColor;
    QString m_inactiveHighlightColor;
    QString m_observablePropertyName;
    QTimer* m_updateTimer;
    SelectionMode m_selectionMode;

    // QQmlParserStatus interface
public:
    void componentComplete() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // DATAGRID_H
