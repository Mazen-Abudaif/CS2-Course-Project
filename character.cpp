#include "character.h"

Character::Character(int health) : health(health)
{

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