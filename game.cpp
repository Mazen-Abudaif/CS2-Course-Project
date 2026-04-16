#include "game.h"
#include "mainmenu.h"
#include "level1.h"
#include <QMessageBox>
#include "characterselect.h"

Game::Game(int width,int height)
{
    //disable scroll wheel horrizontly and verticly
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // setting the resolution
    this->setFixedSize(width, height);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    gameScene = new Mainmenu(this) ;
    this->setScene(gameScene) ;

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

        pair<int,int> player_pos_grid = {newRow, newCol} ;

        // returns new position after offsetting
        pair<int,int> newPos = room->calcScenePosition(newRow, newCol);

        int px = newPos.first + (room->get_tile_size() - player->pixmap().width())/2 ;
        int py = newPos.second + (room->get_tile_size() - player->pixmap().height())/2 ;

        player->setScenePosition(px, py);

        if (player_pos_grid == room-> trap_places[0] || player_pos_grid == room-> trap_places[1] )
        {
            level_1 -> triggerDamageeffect() ;

            player -> decreaseHealth() ;

            level_1 -> updateHearts(player) ;

            if(player->isDead())
            {
                QMessageBox::StandardButton reply = QMessageBox::question
                    (this, "❌ Game Over ❌ ","You Died 🏴‍☠️\n Restart the Game?", QMessageBox::Yes | QMessageBox::No );
                if(reply==QMessageBox::Yes)
                    restart() ;
                else
                    {
                        gamescene = new Mainmenu(this) ;
                        this->setScene(gamescene) ;
                    }
            }
        }
    }
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
    gamescene = new QGraphicsScene() ;
    gamescene ->setSceneRect(0,0,1280,720) ;

    level_1 = new Level1(gamescene, this) ;
    level_1 -> initialise() ;

    this -> setScene(gamescene) ;
    this->setFocus() ;
}

void Game::restart()
{
    if(level_1)
    {
        delete level_1 ;
        level_1 = nullptr ;
    }

    // creating new scene
    gamescene = new QGraphicsScene() ;
    gamescene -> setSceneRect(0,0,1280,720) ;

    // creating the level from the beginning
    level_1 = new Level1(gamescene,this) ;
    level_1 -> initialise() ;

    // set the game scene
    this -> setScene(gamescene) ;
    this -> setFocus() ;
}









void Game::openCharacterSelect()
{
    Characterselect* characterSelectScene = new Characterselect(this);
    this->setScene(characterSelectScene);
}

