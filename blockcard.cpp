#include "blockcard.h"

Blockcard::Blockcard(int effectValue) : Card("block", CardType::Block, effectValue)
{

}

void Blockcard::use()
{

}

int Blockcard::getDamage() const
{
    return effectValue;
}
