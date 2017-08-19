#include "observableobjecteventargs.h"

ObservableObjectEventArgs::ObservableObjectEventArgs(QObject *parent) : QObject(parent)
{
    m_object = NULL;
    m_row = 0;
}

int ObservableObjectEventArgs::row() const
{
    return m_row;
}

QObject *ObservableObjectEventArgs::object() const
{
    return m_object;
}

void ObservableObjectEventArgs::setObject(QObject *object)
{
    m_object = object;
}

void ObservableObjectEventArgs::setRow(int row)
{
    m_row = row;
}
