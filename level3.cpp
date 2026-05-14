#include "level3.h"
#include "game.h"

Level3::Level3(QGraphicsScene* scene, Game* game) : Baselevel(scene, game) {}

void Level3::initialise()
{
    QPixmap bg(":/images/Images/background.jpg");
    bg = bg.scaled(1280, 720, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    setBackground(new QGraphicsPixmapItem(bg));

    Baselevel::initialise();
    scene->setSceneRect(0, 0, 1280, 720);
}

void Level3::setupRoom()
{
    room = new Grid(scene, 3, false);
    room->PlaceLevel3Guard1();
    room->PlaceLevel3Guard2();
    room->PlaceLevel3LockedBoss();
}
