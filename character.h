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

    void setGridPosition(int newRow, int newCol); // function to place player on the grid first
    void setScenePosition(int x, int y) ; // since the grid is offset, this
    //function is used to set the player on the scene based on grid position

    int getRow() const;
    int getCol() const;

    // health functions
    int getHealth() const ;
    void setHealth(int x) ;
    void decreaseHealth(int amount = 1) ;
    bool isDead() const ;

protected:
    int row;
    int col ;
    QPixmap skin;

protected:
    QList<Card*> deck;
    QList<Card*> hand;

private:
    int health ;
};

#endif // CHARACTER_H
