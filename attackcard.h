#ifndef ATTACKCARD_H
#define ATTACKCARD_H
#include "card.h"

class Attackcard : public Card
{
public:
    Attackcard(int effectValue);
    void use() override;
};

#endif // ATTACKCARD_H
