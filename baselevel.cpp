#include "baselevel.h"

Baselevel::Baselevel(QGraphicsScene* scene, Game* game) : QObject() , scene(scene), game(game), player()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Baselevel::update);
    timer->start(16);//60 fps
}

void Baselevel::initialise(){
    player = new Player();
    scene->addItem(player);
    player->setPos(100,500);
}

void Baselevel::setBackground(QGraphicsPixmapItem* background){
    scene->addItem(background);
    background->setPos(0,0);
    background->setZValue(-1);
};

void Baselevel::update(){
    game->ensureVisible(player, 500, 0);
};