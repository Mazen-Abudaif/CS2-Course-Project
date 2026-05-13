#include "game.h"
#include "attackcard.h"
#include "blockcard.h"
#include "mainmenu.h"
#include "level1.h"
#include "level4.h"
#include <QMessageBox>
#include "characterselect.h"
#include "combatscene.h"
#include "rewardscene.h"
#include <QPropertyAnimation>

Game::Game(int width,int height)
{
    current_level = 1 ;
    selectedCharacter = "" ;
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

        pair<int,int> player_pos_grid = {newRow, newCol} ;

        // returns new position after offsetting
        pair<int,int> newPos = room->calcScenePosition(newRow, newCol);

        int px = newPos.first + (room->get_tile_size() - player->pixmap().width())/2 ;
        int py = newPos.second + (room->get_tile_size() - player->pixmap().height())/2 ;

        QPointF startPos = player->pos();
        QPointF endPos(px, py);
        int steps = 10;
        QPointF stepSize = (endPos - startPos) / steps;
        int* stepCount = new int(0);
        QTimer* animationTimer = new QTimer();
        connect(animationTimer, &QTimer::timeout, this, [=]() mutable {
            (*stepCount)++;
            player->setPos(player->pos() + stepSize);
            if(*stepCount >= steps){
                animationTimer->stop();
                animationTimer->deleteLater();
                delete stepCount;
            }
        });
        animationTimer->start(10);

        for(size_t i=0 ; i <room->trap_places.size() ; i++)
        {
            if (player_pos_grid == room-> trap_places.at(i))
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
        for(size_t i=0 ; i <room->attack_card_places.size() ; i++)
        {
            if(player_pos_grid == room-> attack_card_places.at(i).first)
            {
                player -> deck.append(new Attackcard(10)) ;
                room -> RemoveCard(player_pos_grid) ;

            }
        }
        for(size_t i=0 ; i <room->block_card_places.size() ; i++)
        {
            if(player_pos_grid == room-> block_card_places.at(i).first)
            {
                player -> deck.append(new Blockcard(5)) ;
                room->RemoveCard(player_pos_grid) ;

            }
        }
        if(room->isPlayerNearby(newRow,newCol))
        {
            openCombat() ;
        }
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

void Game::setSelectedCharacter(QString character)
{
    selectedCharacter = character ;
}

QString Game::getSelectedCharacter() const
{
    return selectedCharacter ;
}

// opens combat scene — reads triggered enemy stats directly from the room
void Game::openCombat()
{
    int bossHp = 60 ;
    int bossImmuneTurns = 0 ;

    if (level_1 != nullptr && level_1->getRoom() != nullptr)
    {
        bossHp = level_1->getRoom()->getTriggeredCombatHp() ;
        bossImmuneTurns = level_1->getRoom()->getTriggeredImmuneTurns() ;
    }

    CombatScene* combatScene = new CombatScene(this, bossHp, bossImmuneTurns);
    combatScene->initialise();
    this->setScene(combatScene);
}
// called on combat win — removes the defeated enemy and checks if more remain
void Game::onCombatWin()
{
    if (level_1 != nullptr && level_1->getRoom() != nullptr)
    {
        level_1->getRoom()->removeTriggeredEnemy() ;

        if (!level_1->getRoom()->allEnemiesDefeated())
        {
            returnToLevel() ;
            return ;
        }
    }

    openReward() ;
}

// called on combat loss — restarts the appropriate level
void Game::onCombatLose()
{
    if (current_level == 4)
        openLevel4() ;
    else
        openLevel1() ;
}

// returns to the current level scene without recreating it
void Game::returnToLevel()
{
    this->setScene(gamescene) ;
    this->setFocus() ;
}

// opens reward scene after player wins combat

void Game::openReward()
{
    RewardScene* rewardScene = new RewardScene(this);
    rewardScene->initialise();
    this->setScene(rewardScene);
}
void Game::openNextLevel()
{
    if (current_level>=5)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this,"Victory!","You have Conquered all Levels!\n\
 Would you like to restart?",QMessageBox::Yes | QMessageBox::No) ;
        if(reply==QMessageBox::Yes)
        {
            openLevel1() ;
            current_level = 1;
        }
        else
        {
            this->close() ;
        }
    }
    if (current_level == 1)
    {
        openLevel2() ;
    }
    if(current_level == 2)
    {
        openLevel3() ;
    }
    if(current_level == 3)
    {
        openLevel4() ;
    }
    if(current_level == 4)
    {
        openLevel5() ;
    }
    current_level++ ;
}

void Game::openLevel2()
{

}
void Game::openLevel3()
{

}
void Game::openLevel4()
{
    if (level_1)
    {
        delete level_1 ;
        level_1 = nullptr ;
    }

    gamescene = new QGraphicsScene() ;
    gamescene->setSceneRect(0, 0, 1280, 720) ;

    level_1 = new Level4(gamescene, this) ;
    level_1->initialise() ;

    this->setScene(gamescene) ;
    this->setFocus() ;
}
void Game::openLevel5()
{

}
void Game::setSelectedCharacter(QString Character)
{
    this->Character = Character;
}
QString Game::getCharacter()
{
    return this->Character;
}