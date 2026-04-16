#include "game.h"
#include "mainmenu.h"
#include "level1.h"

Game::Game(int width, int height)
{

    //disable scroll wheel horrizontly and verticly
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // setting the resolution
    this->setFixedSize(width, height);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    gamescene = new Mainmenu(this) ;
    this->setScene(gamescene) ;

}

void Game::keyPressEvent(QKeyEvent *event)
{
    if(!level_1)
        return ;

    Player* player = level_1 -> getPlayer() ;
    Grid *room = level_1 -> getRoom() ;

    if(!player|| !room)
        return ;

    int newRow = player->getRow();
    int newCol = player->getCol();

    if (event->key() == Qt::Key_Up)
        newRow--;
    else if (event->key() == Qt::Key_Down)
        newRow++;
    else if (event->key() == Qt::Key_Left)
        newCol--;
    else if (event->key() == Qt::Key_Right)
        newCol++;

    if (room->isWalkable(newRow, newCol))
    {
        player->setGridPosition(newRow, newCol);

        QPoint newPos = room->getScenePosition(newRow, newCol);

        int px = newPos.x() + (room->get_tile_size() - player->pixmap().width()) / 2;
        int py = newPos.y() + (room->get_tile_size() - player->pixmap().height()) / 2;

        player->setScenePosition(px, py);
    }
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
    gamescene = new QGraphicsScene() ;
    gamescene ->setSceneRect(0,0,1280,720) ;

    level_1 = new Level1(gamescene, this) ;
    level_1 -> initialise() ;

    this -> setScene(gamescene) ;
    this->setFocus() ;
}

