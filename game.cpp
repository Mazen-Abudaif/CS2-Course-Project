#include "game.h"
#include "mainmenu.h"
#include "level1.h"

Game::Game(int width, int height)
{
    //disable scroll wheel horrizontly and verticly
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //set res
    this->setFixedSize(width, height);

    gamescene = new Mainmenu(this);
    this->setScene(gamescene);
}


void Game::openMenu()
{

    this->setScene(gamescene);
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

