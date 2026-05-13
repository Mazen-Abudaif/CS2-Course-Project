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

    // setting hearts (lives)
    QPixmap heartPixmap(":/images/Images/heart.png") ;
    heartPixmap = heartPixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int spacing = 22;
    for (int i = 0; i < player->getHealth(); i++)
    {
        QGraphicsPixmapItem* heart = new QGraphicsPixmapItem(heartPixmap);

        heart->setPos(580+i*spacing,110);
        heart->setZValue(200);

        scene->addItem(heart);
        hearts.push_back(heart);
    }

}

void Baselevel::updateHearts(Player *player)
{
    int currentHealth = player->getHealth();

    for (int i = 0; i < hearts.size(); i++)
    {
        if (i < currentHealth)
            hearts[i]->setVisible(true);
        else
            hearts[i]->setVisible(false);
    }
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

