#include "boss.h"
#include "attackcard.h"
#include "blockcard.h"
#include "consumable.h"
#include <cstdlib>

Boss::Boss(int health, BossType type, int damage) : Character(health), type(type)
{
    deck.append(new Attackcard(damage));
    deck.append(new Blockcard(100));
    deck.append(new Consumable("Heal",30));
}

void Boss::aiPlayCard()
{
    if (deck.isEmpty())
        return;
    playCard(rand() % deck.size());
}

BossType Boss::getType() const
{
    return type ;
}
