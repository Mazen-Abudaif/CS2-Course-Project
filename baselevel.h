#ifndef BASELEVEL_H
#define BASELEVEL_H
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include "player.h"
#include "grid.h"
#include <vector>
#include <QGraphicsPixmapItem>


class Game ;

class Baselevel : public QObject
{
    Q_OBJECT
public:
    Baselevel(QGraphicsScene* scene, Game* game); // constructor
    virtual void initialise() = 0; // makes game a purely virtual function

    void setBackground(QGraphicsPixmapItem* background);

    void triggerDamageeffect() ; // function to trigger screen effect when walked on trap

    Player* getPlayer() const;
    Grid* getRoom() const;
    QGraphicsScene* getScene();
    Game* getGame();

    void updateHearts(Player *player); // updating hearts (lives) when character is damaged

private slots:
    void update();

protected:
    Grid *room ;
    QGraphicsScene* scene;
    Game* game;
    QTimer* timer;
    Player* player ;
    // adds effect when walked on trap
    QGraphicsRectItem* damageOverlay;
    vector<QGraphicsPixmapItem*> hearts; // "player lives"
};

#endif // BASELEVEL_H
