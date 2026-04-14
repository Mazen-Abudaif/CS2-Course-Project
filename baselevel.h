#ifndef BASELEVEL_H
#define BASELEVEL_H
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include "game.h"
#include "player.h"

class Baselevel : public QObject
{
    Q_OBJECT
public:
    Baselevel(QGraphicsScene* scene, Game* game);
    virtual void initialise() = 0;
    void setBackground(QGraphicsPixmapItem* background);

private slots:
    void update();

private:
    QGraphicsScene* scene;
    Game* game;
    QTimer* timer;
    Player* player;
};

#endif // BASELEVEL_H
