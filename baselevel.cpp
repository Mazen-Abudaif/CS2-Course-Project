#include "baselevel.h"
#include "game.h"

Baselevel::Baselevel(QGraphicsScene* scene, Game* game) : QObject() , room(nullptr), scene(scene), game(game), player(nullptr)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Baselevel::update);
    timer->start(16);//60 fps
}

void Baselevel::initialise(){
    room = new Grid(scene) ;
    // creating player
    player = new Player();
    scene->addItem(player);
    player->setGridPosition(1,1);
    player->setZValue(2) ;

    QPoint pos = room->getScenePosition(1, 1);

    int px = pos.x() + (room->get_tile_size() - player->pixmap().width()) / 2;
    int py = pos.y() + (room->get_tile_size() - player->pixmap().height()) / 2;

    player->setScenePosition(px, py);

}

void Baselevel::setBackground(QGraphicsPixmapItem* background){
    scene->addItem(background);
    background->setPos(0,0);
    background->setZValue(-1);
};

Player* Baselevel::getPlayer() const
{
    return player;
}

Grid* Baselevel::getRoom() const
{
    return room;
}

void Baselevel::update(){

        if (!game)
        {
            qDebug() << "game is null";
            return;
        }

        if (!player)
        {
            qDebug() << "player is null";
            return;
        }

        game->ensureVisible(player, 500, 0);
    }