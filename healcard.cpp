#include "healcard.h"

HealCard::HealCard(int effectValue):Card("heal",CardType::Heal , effectValue)
{

}

void HealCard::use()
{

}

int HealCard::getDamage() const
{
    return effectValue;
}



