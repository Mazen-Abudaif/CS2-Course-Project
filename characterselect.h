#ifndef CHARACTERSELECT_H
#define CHARACTERSELECT_H
#include <QGraphicsScene>
#include "game.h"

class Characterselect : public QGraphicsScene
{
    Q_OBJECT
public:
    Characterselect(Game* game);

public slots:
    void selectMage();
    void selectWarrior();

private:
    Game* game;
};

#endif // CHARACTERSELECT_H
