#include "level4.h"
#include "game.h"
#include "cardtype.h"
#include <QGraphicsPixmapItem>

Level4::Level4(QGraphicsScene* scene, Game* game) : Baselevel(scene, game) {}

void Level4::setupRoom()
{
    // current_level=4 triggers interior wall barriers in initialize_room, spawnBoss=false uses fast+tanky enemies
    room = new Grid(scene, 4, false) ;

    // extra traps and an additional attack card on top of what the constructor spawns
    room->SpawnTraps(2) ;
    room->SpawnCards(CardType::Attack, 1) ;
}

void Level4::initialise()
{
    QPixmap backgroundpixmap(":/images/Images/level4_background.png") ;
    QSize targetSize(1280, 720) ;
    QPixmap scaledPixmap = backgroundpixmap.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation) ;
    QGraphicsPixmapItem* background = new QGraphicsPixmapItem(scaledPixmap) ;
    Baselevel::setBackground(background) ;

    Baselevel::initialise() ;
    scene->setSceneRect(0, 0, 1280, 720) ;
}
