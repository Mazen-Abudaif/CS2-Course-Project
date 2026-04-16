#ifndef CHARACTER_H
#define CHARACTER_H
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QList>
#include "card.h"

class Character : public QGraphicsPixmapItem
{
public:
    Character(int health);
    void drawCard();
    void playCard(int index);

    void setGridPosition(int newRow, int newCol);
    void setScenePosition(int x, int y) ;
    int getRow() const;
    int getCol() const;

protected:
    int row;
    int col ;
    QPixmap skin;
    int health;
    QList<Card*> deck;
    QList<Card*> hand;
};

#endif // CHARACTER_H
