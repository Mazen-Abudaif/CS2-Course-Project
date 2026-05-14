#ifndef BASELEVEL_H
#define BASELEVEL_H
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include "player.h"
#include "grid.h"
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QProgressBar>

class Game ;

class Baselevel : public QObject
{
    Q_OBJECT
public:
    Baselevel(QGraphicsScene* scene, Game* game); // constructor
    virtual void initialise() = 0; // makes game a purely virtual function
    virtual void setupRoom() ; // override in subclasses to change enemy setup

    void setBackground(QGraphicsPixmapItem* background);

    void triggerDamageeffect() ; // function to trigger screen effect when walked on trap

    Player* getPlayer() const;
    Grid* getRoom() const;
    QGraphicsScene* getScene();
    Game* getGame();
    virtual ~Baselevel() {} ;
    void updateHpBar() ;
private slots:
    void update();

protected:
    Grid *room ;
    QGraphicsScene* scene;
    Game* game;
    QTimer* timer;
    Player* player;
    // adds effect when walked on trap
    QGraphicsRectItem* damageOverlay;
    int playerStartRow = 1;
    int playerStartCol = 1;
    QProgressBar* hpBar = nullptr;
    QGraphicsProxyWidget* hpProxy = nullptr;

};

#endif // BASELEVEL_H
