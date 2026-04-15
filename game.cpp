#include "game.h"
#include "mainmenu.h"
#include "level1.h"
#include "combatscene.h"
#include "rewardscene.h"

Game::Game(int width, int height)
{
    //disable scroll wheel horrizontly and verticly
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //set res
    this->setFixedSize(width, height);

    gameScene = new Mainmenu(this);
    this->setScene(gameScene);
}


void Game::openMenu()
{

    this->setScene(gameScene);
    this->show();

}


void Game::closeMenu()
{
    this->hide();
}

void Game::openLevel1()
{
    QGraphicsScene* level1Scene = new QGraphicsScene();
    level1Scene->setSceneRect(0, 0, 1280, 720);
    Level1* level = new Level1(level1Scene, this);
    level->initialise();
    this->setScene(level1Scene);
}
// opens combat scene
void Game::openCombat() {
    CombatScene* combatScene = new CombatScene(this);
    combatScene->initialise();
    this->setScene(combatScene);
}
// opens reward scene after player wins combat
void Game::openReward()
{
    RewardScene* rewardScene = new RewardScene(this);
    rewardScene->initialise();
    this->setScene(rewardScene);
}
