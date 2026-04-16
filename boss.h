#ifndef BOSS_H
#define BOSS_H
#include "character.h"

class Boss : public Character
{
public:
    Boss(int health);
    void aiPlayCard();

private:
};

#endif // BOSS_H
