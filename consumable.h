#ifndef CONSUMABLE_H
#define CONSUMABLE_H
#include "card.h"

class Consumable : public Card
{
public:
    Consumable(QString name, int effectValue);
    void use() override;

};

#endif // CONSUMABLE_H
