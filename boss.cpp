#include "boss.h"
#include "attackcard.h"
#include "blockcard.h"
#include "consumable.h"

Boss::Boss(int health) : Character(health)
{
    deck.append(new Attackcard(50));
    deck.append(new Blockcard(100));
    deck.append(new Consumable("Heal",30));
}

void Boss::aiPlayCard()
{
    playCard(0);
}
