#include "baselevel.h"
#include "game.h"
#include <QLabel>
#include <QProgressBar>
#include <QGraphicsProxyWidget>
#include "healcard.h"
#include "attackcard.h"
#include "blockcard.h"
#include <QSettings>

Baselevel::Baselevel(QGraphicsScene* scene, Game* game) : QObject() , room(nullptr), scene(scene), game(game), player(nullptr)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Baselevel::update);
    timer->start(16);//60 fps
}

void Baselevel::setupRoom()
{
    room = new Grid(scene,game->current_level,true) ;
}

void Baselevel::initialise(){

    setupRoom() ;
    // creating player
    player = new Player(game->getSelectedCharacter());
    scene->addItem(player);

    player->setGridPosition(playerStartRow, playerStartCol);
    player->setZValue(20) ;

    // get position of character after offsetting
    pair<int,int> pos = room->calcScenePosition(playerStartRow, playerStartCol);

    int px = pos.first + (room->get_tile_size()- player->pixmap().width())/2 ;
    int py = pos.second + (room->get_tile_size()- player->pixmap().height())/2 ;

    player->setScenePosition(px,py);

    hpBar = new QProgressBar();
    hpBar->setRange(0, 100);
    hpBar->setValue(player->getHealth());
    hpBar->setFormat("%v / %m");

    hpProxy = scene->addWidget(hpBar);
    hpProxy->setPos(50, 20);
    hpProxy->setZValue(20);

    // setting damage effect
    damageOverlay = new QGraphicsRectItem(0, 0, 1280, 720);
    damageOverlay->setBrush(QColor(255, 0, 0, 120)); // red with transparency
    damageOverlay->setPen(Qt::NoPen);
    damageOverlay->setZValue(100); // on top of everything
    damageOverlay->setVisible(false);

    scene->addItem(damageOverlay);
    // after player is created and positioned...

    QString reward = game->getRewardCard();
    if (reward == "Greater Heal")
        player->deck.append(new HealCard(20));
    else if (reward == "Dagger")
        player->deck.append(new Attackcard(5));

    QSettings settings("MyGame", "SaveData");
    if (settings.contains("hp"))
    {
        player->setHealth(settings.value("hp").toInt());
        updateHpBar();

        QStringList cardList = settings.value("deck").toStringList();
        for (const QString& entry : cardList)
        {
            QStringList parts = entry.split(":");
            if (parts.size() != 2) continue;
            QString type = parts[0];
            int value = parts[1].toInt();

            if (type == "Attack")
                player->deck.append(new Attackcard(value));
            else if (type == "Block")
                player->deck.append(new Blockcard(value));
            else if (type == "Heal")
                player->deck.append(new HealCard(value));
        }

        settings.remove("hp");
        settings.remove("deck");
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

void Baselevel::updateHpBar()
{
    if (hpBar && player)
        hpBar->setValue(player->getHealth());
}