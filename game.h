#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QKeyEvent>
#include <QString>
#include "baselevel.h"

class Game : public QGraphicsView {
    Q_OBJECT //needed for qt
public:
    Game(int w, int h); // constructor with width and height paramters
    void closeMenu(); // functions to open main menu and close it
    void openMenu();
    void openLevel1(); // function to open level 1 in the game
    void openLevel2() ;
    void openLevel3() ;
    void openLevel4() ;
    void openLevel5() ;
    void restart() ; // function to restart level
    void openCharacterSelect();
    void openCombat();
    void openReward();
    void onCombatWin();
    void onCombatLose();
    void returnToLevel();
    void setSelectedCharacter(QString character);
    QString getSelectedCharacter() const;
    void setRewardCard(QString reward);
    QString getRewardCard() const;
    void openNextLevel() ;
    Player* getPlayer() ;
    bool hasSave();
    void loadSave();

    int current_level ;

private:
    QGraphicsScene* gamescene;
    Baselevel* level_1 ;
    QString Character;
    Player *p ;
    QString rewardCard;



protected:
    void keyPressEvent(QKeyEvent *event) override;
};


#endif // GAME_H
