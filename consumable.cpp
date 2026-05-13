#include "consumable.h"

Consumable::Consumable(QString name, int effectValue) : Card(name, CardType::Consumable, effectValue)
{

}

void Consumable::use()
{

}