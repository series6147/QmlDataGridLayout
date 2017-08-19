#include "itemsmodel.h"

ItemsModel::ItemsModel(QObject *parent) : QAbstractListModel(parent)
{
}

void ItemsModel::populate(QList<QString> roles, QList<QList<QVariant>> items)
{
    auto i = 0;

    for (auto role : roles)
    {
        m_roleNames[i++] = role.toLocal8Bit();
    }

    m_items = items;
}

int ItemsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_items.size();
}

QVariant ItemsModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();

    if (i < 0 || i >= m_items.size())
        return QVariant(QVariant::Invalid);

    if (m_roleNames.contains(role))
    {
        return m_items[i][role];
    }
    return QVariant();
}

bool ItemsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int i = index.row();

    if (i < 0 || i >= m_items.size() || role < 0 || role >= m_roleNames.size())
    {
        return false;
    }

    m_items[i][role] = value;
    return true;
}

QHash<int, QByteArray> ItemsModel::roleNames() const
{
    return m_roleNames;
}
