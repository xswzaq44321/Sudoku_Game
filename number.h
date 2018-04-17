#ifndef NUMBER_H
#define NUMBER_H

#include <QObject>
#include <QGraphicsPixmapItem>

class number: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    number();
    number(int x, int y);
};

#endif // NUMBER_H
