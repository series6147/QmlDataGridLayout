#ifndef OBSERVABLEOBJECTEVENTARGS_H
#define OBSERVABLEOBJECTEVENTARGS_H

#include <QObject>

class ObservableObjectEventArgs : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow)
    Q_PROPERTY(QObject* object READ object WRITE setObject)
public:
    explicit ObservableObjectEventArgs(QObject *parent = nullptr);
    int row() const;
    QObject* object() const;

public slots:
    void setObject(QObject* object);
    void setRow(int row);

private:
    int m_row;
    QObject* m_object;
};

#endif // OBSERVABLEOBJECTEVENTARGS_H
