#ifndef BLOCKCARD_H
#define BLOCKCARD_H
#include "card.h"

class Blockcard : public Card
{
public:
    Blockcard(int effectValue);
    void use() override;
    int getDamage() const override;
};

#endif // BLOCKCARD_H
