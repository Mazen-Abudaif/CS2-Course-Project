#ifndef CARD_H
#define CARD_H
#include <QString>

class Card
{
public:
    Card(QString name, QString type, int effectValue);
    virtual void use() = 0;
protected:
    QString name;
    QString type;
    int effectValue;
};

#endif // CARD_H
