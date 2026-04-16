#include "character.h"

Character::Character(int health) : health(health)
{
    row = 1;
    col = 1;
}

void Character::drawCard()
{
    Card* card = deck.takeFirst();
    hand.append(card);
}

void Character::playCard(int index)
{
    hand[index]->use();
    hand.removeAt(index);
}

void Character::setGridPosition(int newRow, int newCol)
{
    row = newRow;
    col = newCol;
}

void Character::setScenePosition(int x, int y)
{
    setPos(x,y) ;
}

int Character::getRow() const
{
    return row;
}

int Character::getCol() const
{
    return col;
}

int Character :: getHealth() const
{
    return health ;
}
void Character :: setHealth(int x)
{
    health = x;
}
void Character :: decreaseHealth(int amount)
{
    health -= amount ;
    if(health<0)
        health = 0 ;
}
bool Character :: isDead() const
{
    return(health<=0) ;
}

void Character::drawCard()
{
    Card* card = deck.takeFirst();
    hand.append(card);
}

void Character::playCard(int index)
{
    hand[index]->use();
    hand.removeAt(index);
}