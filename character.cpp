#include "character.h"

Character::Character()
{
    row = 1;
    col = 1;
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