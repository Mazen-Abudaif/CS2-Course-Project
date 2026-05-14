#include "level2.h"
#include "game.h"

Level2::Level2(QGraphicsScene* scene, Game* game) : Baselevel(scene,game)
{

}

void Level2::initialise()
{
    QPixmap bg(":/images/Images/background.jpg") ;
    bg = bg.scaled(1280, 720, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation) ;
    setBackground(new QGraphicsPixmapItem(bg)) ;

    Baselevel::initialise();
    scene->setSceneRect(0, 0, 1280, 720);
}

void Level2::setupRoom()
{
    room = new Grid(scene, 2, false);
    room->PlaceLevel2Enemy1();
    room->PlaceLevel2Enemy2();
}