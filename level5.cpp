#include "level5.h"
#include "game.h"

Level5::Level5(QGraphicsScene* scene, Game* game) : Baselevel(scene, game) {}

void Level5::initialise(){

    playerStartRow = 15;
    playerStartCol = 1;

    Baselevel::initialise();

    player -> setHealth(7) ; // setting health of the player
    room->updateDarkness(15, 1);

    QPixmap backgroundpixmap(":/images/Images/level_5_background.png");

    QSize targetSize(1280, 720);
    QPixmap scaledPixmap = backgroundpixmap.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* background1 = new QGraphicsPixmapItem(scaledPixmap);
    Baselevel::setBackground(background1);
    background1->setZValue(-10); ;

    scene->setSceneRect(0, 0, 1280, 720);
}
