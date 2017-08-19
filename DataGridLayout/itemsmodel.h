#ifndef ITEMSMODEL_H
#define ITEMSMODEL_H

#include <QAbstractItemModel>
#include <QObject>

class ItemsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ItemsModel(QObject *parent = nullptr);
    void populate(QList<QString> roles, QList<QList<QVariant>> items);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    QHash<int, QByteArray> m_roleNames;
    QList<QList<QVariant>> m_items;

    // QAbstractItemModel interface
public:
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // ITEMSMODEL_H
