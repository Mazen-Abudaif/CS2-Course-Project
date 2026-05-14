#ifndef HEALCARD_H
#define HEALCARD_H
#include "card.h"

class HealCard : public Card
{
public:
    HealCard(int effectValue) ;
    void use() override ;
    int getDamage() const override ;
};

#endif // HEALCARD_H
