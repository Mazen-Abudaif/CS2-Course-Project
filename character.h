#ifndef CHARACTER_H
#define CHARACTER_H
#include <QGraphicsPixmapItem>
#include <QPixmap>

class Character : public QGraphicsPixmapItem
{
public:
    Character();

protected:
    QPixmap skin;
};

#endif // CHARACTER_H
