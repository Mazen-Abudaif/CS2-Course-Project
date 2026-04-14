#include "level1.h"

Level1::Level1(QGraphicsScene* scene, Game* game) : Baselevel(scene, game) {}

void Level1::initialise(){

    QPixmap backgroundpixmap(":/images/Images/background.jpg");
    QSize targetSize(1280, 720);
    QPixmap scaledPixmap = backgroundpixmap.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* background1 = new QGraphicsPixmapItem(scaledPixmap);
    Baselevel::setBackground(background1);

    Baselevel::initialise();

};
