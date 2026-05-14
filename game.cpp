#include "game.h"
#include "attackcard.h"
#include "blockcard.h"
#include "healcard.h"
#include "level5.h"
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
    Character = "" ;
    rewardCard = "" ;
    level_1 = nullptr ;
    //disable scroll wheel horrizontly and verticly
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // setting the resolution
    this->setFixedSize(width, height);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    level_1 = nullptr;
    gamescene = new Mainmenu(this);
    this->setScene(gamescene);

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
        player->setZValue(20);

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

            player -> decreaseHealth(10) ;

            level_1->updateHpBar();

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
                if(getSelectedCharacter()=="mage")
                {player -> deck.append(new Attackcard(10)) ;}
                else if(getSelectedCharacter()=="warrior")
                {
                    {player -> deck.append(new Attackcard(5)) ;}
                }
                room -> RemoveCard(player_pos_grid) ;

            }
        }
        for(size_t i=0 ; i <room->block_card_places.size() ; i++)
        {
            if(player_pos_grid == room-> block_card_places.at(i).first)
            {
                if(getSelectedCharacter()=="mage")
                {player -> deck.append(new Blockcard(5)) ;}
                else if(getSelectedCharacter()=="warrior")
                {
                    {player -> deck.append(new Blockcard(20));}
                }
                room->RemoveCard(player_pos_grid) ;

            }
        }
        for(size_t i=0 ; i <room->heal_card_places.size() ; i++)
        {
            if(player_pos_grid == room-> heal_card_places.at(i).first)
            {
                if(getSelectedCharacter()=="mage")
                {player -> deck.append(new HealCard(10)) ;}
                else if(getSelectedCharacter()=="warrior")
                {
                    {player -> deck.append(new Blockcard(20));}
                }
                room->RemoveCard(player_pos_grid) ;

            }
        }
        p = player ;
        if(room->isPlayerNearby(newRow,newCol))
        {
            openCombat() ;
            return ;
        }
    }
    if(current_level==5)
        room->updateDarkness(newRow, newCol);
}

void Game::openMenu()
{

    gamescene = new Mainmenu(this);
    this->setScene(gamescene);
    this->show();

}


void Game::closeMenu()
{
    this->hide();
}

void Game::openLevel1()
{
    current_level = 1 ;
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
        delete level_1;
        level_1 = nullptr;
    }

    if(current_level == 1)
        openLevel1();
    else if(current_level == 2)
        openLevel2() ;
    else if(current_level == 3)
        openLevel3() ;
    else if(current_level == 4)
        openLevel4();
    else if(current_level == 5)
        openLevel5();
}

void Game::openCharacterSelect()
{
    Characterselect* characterSelectScene = new Characterselect(this);
    this->setScene(characterSelectScene);
}

void Game::setSelectedCharacter(QString character)
{
    Character = character ;
}

QString Game::getSelectedCharacter() const
{
    return Character ;
}

void Game::setRewardCard(QString reward)
{
    rewardCard = reward ;
}

QString Game::getRewardCard() const
{
    return rewardCard ;
}

// opens combat scene — reads triggered enemy stats directly from the room
void Game::openCombat()
{
    Player* currentPlayer = level_1->getPlayer();

    if (currentPlayer == nullptr)
        return ;

    p = currentPlayer;

    bool hasUsableCard = false;

    for (Card* card : currentPlayer->deck)
    {
        if (card->getType() == CardType::Attack ||
            card->getType() == CardType::Block ||
            card->getType() == CardType::Heal)
        {
            hasUsableCard = true;
            break;
        }
    }

    if (currentPlayer == nullptr || !hasUsableCard)
    {
        QMessageBox::StandardButton reply =
            QMessageBox::question(this,
                                  "Game Over",
                                  "You entered combat with no usable cards.\nRestart the game?",
                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
            restart();
        else
            openMenu();

        return;
    }

    int bossHp ;
    if(current_level==5)
        bossHp = 150 ;
        else
            bossHp = 60;
    int bossImmuneTurns = 0;

    if (level_1 != nullptr && level_1->getRoom() != nullptr)
    {
        bossHp = level_1->getRoom()->getTriggeredCombatHp();
        bossImmuneTurns = level_1->getRoom()->getTriggeredImmuneTurns();
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
    if (current_level == 1)
    {
        openLevel1() ;
    }
    if(current_level == 2)
    {
        openLevel2() ;
    }
    if(current_level == 3)
    {
        openLevel3() ;
    }
    if(current_level == 4)
    {
        openLevel4() ;
    }
    if(current_level ==5)
    {
        openLevel5() ;
    }
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
        return ;
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
}

void Game::openLevel2()
{
    current_level = 2;
}
void Game::openLevel3()
{
    current_level = 3;
}
void Game::openLevel4()
{
    current_level = 4;
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
    current_level = 5;
    if (level_1)
    {
        delete level_1 ;
        level_1 = nullptr ;
    }
    gamescene = new QGraphicsScene(this) ;
    gamescene->setSceneRect(0, 0, 1280, 720) ;
    level_1 = new Level5(gamescene, this) ;
    level_1->initialise() ;
    this->setScene(gamescene) ;
    this->setFocus() ;
}

Player* Game::getPlayer()
{
    return p ;
}