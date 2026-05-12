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
    player = new Player("mage");
    scene->addItem(player);
    player -> setHealth(5) ; // setting health of the player

    // setting initial position of character
    player->setGridPosition(1,1);
    player->setZValue(2) ;

    // get position of character after offsetting
    pair<int,int> pos = room->calcScenePosition(1, 1);

    int px = pos.first + (room->get_tile_size()- player->pixmap().width())/2 ;
    int py = pos.second + (room->get_tile_size()- player->pixmap().height())/2 ;

    player->setScenePosition(px,py);


    // setting damage effect
    damageOverlay = new QGraphicsRectItem(0, 0, 1280, 720);
    damageOverlay->setBrush(QColor(255, 0, 0, 120)); // red with transparency
    damageOverlay->setPen(Qt::NoPen);
    damageOverlay->setZValue(100); // on top of everything
    damageOverlay->setVisible(false);

    scene->addItem(damageOverlay);

    playerHpLabel = new QLabel("HP");
    playerHpBar = new QProgressBar();

    playerHpBar->setRange(0, 5);              // because you set player health to 5
    playerHpBar->setValue(player->getHealth());
    playerHpBar->setFormat("%v / %m");

    QGraphicsProxyWidget* labelProxy = scene->addWidget(playerHpLabel);
    labelProxy->setPos(580, 80);

    QGraphicsProxyWidget* barProxy = scene->addWidget(playerHpBar);
    barProxy->setPos(580, 110);

}

void Baselevel::updateHpBar(Player* player)
{
    if (!playerHpBar || !player)
        return;

    playerHpBar->setValue(player->getHealth());
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

void Baselevel::triggerDamageeffect()
{
    damageOverlay->setVisible(true);

    QTimer::singleShot(150, this, [this]() {
        damageOverlay->setVisible(false);
    });
    game->ensureVisible(player, 500, 0);
};

QGraphicsScene* Baselevel::getScene() {
    return scene;
}

Game* Baselevel::getGame() {
    return game;
}

