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