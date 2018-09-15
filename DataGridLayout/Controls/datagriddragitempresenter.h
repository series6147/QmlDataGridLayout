#ifndef DATAGRIDDRAGITEMPRESENTER_H
#define DATAGRIDDRAGITEMPRESENTER_H

#include <QImage>
#include <QPainter>
#include <QQuickPaintedItem>

class DataGridDragItemPresenter : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int origX READ origX WRITE setOrigX)
    Q_PROPERTY(int origY READ origY WRITE setOrigY)
    Q_PROPERTY(QImage image READ image WRITE setImage)
public:
    explicit DataGridDragItemPresenter(QQuickItem *parent = nullptr);
    int origX() const;
    int origY() const;
    QImage image() const;

public slots:
    void setImage(const QImage &image);
    void setOrigX(int origX);
    void setOrigY(int origY);

private:
    int m_origX;
    int m_origY;
    QImage m_image;

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter) override;
};

#endif // DATAGRIDDRAGITEMPRESENTER_H
