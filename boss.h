#ifndef BOSS_H
#define BOSS_H
#include "character.h"

enum class BossType { Regular, Fast, Tanky };

class Boss : public Character
{
public:
    Boss(int health, BossType type = BossType::Regular);
    void aiPlayCard();
    BossType getType() const ;

private:
    BossType type ;
};

#endif // BOSS_H
