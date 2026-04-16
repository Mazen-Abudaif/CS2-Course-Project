#include "level1.h"

Level1::Level1(QGraphicsScene* scene, Game* game) : Baselevel(scene, game) {}

void Level1::initialise(){


    QPixmap backgroundpixmap(":/images/Images/level 1_background.png");
    QSize targetSize(1280, 720);
    QPixmap scaledPixmap = backgroundpixmap.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* background1 = new QGraphicsPixmapItem(scaledPixmap);
    Baselevel::setBackground(background1);

    Baselevel::initialise();
    scene->setSceneRect(0, 0, 1280, 720);

};
