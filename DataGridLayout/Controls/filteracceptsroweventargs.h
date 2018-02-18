#ifndef FILTERACCEPTSROWEVENTARGS_H
#define FILTERACCEPTSROWEVENTARGS_H

#include <QModelIndex>
#include <QObject>

class FilterAcceptsRowEventArgs : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool accepted READ accepted WRITE setAccepted)
    Q_PROPERTY(int sourceRow READ sourceRow WRITE setSourceRow)
    Q_PROPERTY(QModelIndex sourceParent READ sourceParent WRITE setSourceParent)
public:
    explicit FilterAcceptsRowEventArgs(QObject *parent = nullptr);
    explicit FilterAcceptsRowEventArgs(int sourceRow, const QModelIndex &sourceParent, QObject *parent = nullptr);
    bool accepted() const;
    int sourceRow() const;
    QModelIndex sourceParent() const;

public slots:
    void setAccepted(bool accepted);
    void setSourceRow(int sourceRow);
    void setSourceParent(QModelIndex sourceParent);

private:
    bool m_accepted;
    int m_sourceRow;
    QModelIndex m_sourceParent;
};

#endif // FILTERACCEPTSROWEVENTARGS_H
