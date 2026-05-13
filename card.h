#ifndef CARD_H
#define CARD_H
#include "cardtype.h"
#include <QString>

class Card
{
public:
    Card(QString name, CardType type, int effectValue);
    virtual void use() = 0;
protected:
    QString name;
    CardType type; // changed this to a enum
    int effectValue;
};

#endif // CARD_H
