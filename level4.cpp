#include "level4.h"
#include "game.h"
#include <QGraphicsPixmapItem>

Level4::Level4(QGraphicsScene* scene, Game* game) : Baselevel(scene, game) {}

void Level4::setupRoom()
{
    // level 4 has two enemy types instead of the regular boss
    // PlaceFastEnemy and PlaceTankyEnemy are called inside the Grid constructor
    room = new Grid(scene, false) ;
}

void Level4::initialise()
{
    // TODO: replace with a level 4 background when available
    QPixmap backgroundpixmap(":/images/Images/level 1_background.png") ;
    QSize targetSize(1280, 720) ;
    QPixmap scaledPixmap = backgroundpixmap.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation) ;
    QGraphicsPixmapItem* background = new QGraphicsPixmapItem(scaledPixmap) ;
    Baselevel::setBackground(background) ;

    Baselevel::initialise() ;
    scene->setSceneRect(0, 0, 1280, 720) ;
}
