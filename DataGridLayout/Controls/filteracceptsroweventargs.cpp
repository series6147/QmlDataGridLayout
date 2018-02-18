#include "filteracceptsroweventargs.h"

FilterAcceptsRowEventArgs::FilterAcceptsRowEventArgs(QObject *parent) : QObject(parent)
{
    m_accepted = true;
}

FilterAcceptsRowEventArgs::FilterAcceptsRowEventArgs(int sourceRow, const QModelIndex &sourceParent, QObject *parent) : QObject(parent)
{
    m_accepted = true;
    m_sourceParent = sourceParent;
    m_sourceRow = sourceRow;
}

bool FilterAcceptsRowEventArgs::accepted() const
{
    return m_accepted;
}

int FilterAcceptsRowEventArgs::sourceRow() const
{
    return m_sourceRow;
}

QModelIndex FilterAcceptsRowEventArgs::sourceParent() const
{
    return m_sourceParent;
}

void FilterAcceptsRowEventArgs::setAccepted(bool accepted)
{
    m_accepted = accepted;
}

void FilterAcceptsRowEventArgs::setSourceParent(QModelIndex sourceParent)
{
    m_sourceParent = sourceParent;
}

void FilterAcceptsRowEventArgs::setSourceRow(int sourceRow)
{
    m_sourceRow = sourceRow;
}
