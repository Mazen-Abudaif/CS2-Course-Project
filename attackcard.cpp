#include "attackcard.h"

Attackcard::Attackcard(int effectValue) : Card("Attack", CardType::Attack , effectValue)
{

}

void Attackcard::use()
{

}

int Attackcard::getDamage() const
{
    return effectValue;
}