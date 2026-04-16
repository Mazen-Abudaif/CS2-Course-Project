#ifndef BASELEVEL_H
#define BASELEVEL_H
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include "player.h"
#include "grid.h"

class Game ;

class Baselevel : public QObject
{
    Q_OBJECT
public:
    Baselevel(QGraphicsScene* scene, Game* game);
    virtual void initialise() = 0;
    void setBackground(QGraphicsPixmapItem* background);

    Player* getPlayer() const;
    Grid* getRoom() const;

private slots:
    void update();

protected:
    Grid *room ;
    QGraphicsScene* scene;
    Game* game;
    QTimer* timer;
    Player* player;
};

#endif // BASELEVEL_H
