#include "datagriddragitempresenter.h"

DataGridDragItemPresenter::DataGridDragItemPresenter(QQuickItem *parent) : QQuickPaintedItem(parent)
{
}

int DataGridDragItemPresenter::origX() const
{
    return m_origX;
}

int DataGridDragItemPresenter::origY() const
{
    return m_origY;
}

QImage DataGridDragItemPresenter::image() const
{
    return m_image;
}

void DataGridDragItemPresenter::paint(QPainter *painter)
{
    painter->drawImage(0, 0, m_image);
}

void DataGridDragItemPresenter::setImage(const QImage &image)
{
    m_image = image;

    setHeight(image.height());
    setWidth(image.width());

    update(boundingRect().toRect());
}

void DataGridDragItemPresenter::setOrigX(int origX)
{
    m_origX = origX;
}

void DataGridDragItemPresenter::setOrigY(int origY)
{
    m_origY = origY;
}
