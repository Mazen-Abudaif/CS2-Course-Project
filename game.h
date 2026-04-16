#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QKeyEvent>
#include "level1.h"

class Game : public QGraphicsView {
    Q_OBJECT //needed for qt
public:
    Game(int w, int h); // constructor with width and height paramters
    void closeMenu(); // functions to open main menu and close it
    void openMenu();
    void openLevel1(); // function to open level 1 in the game
    void restart() ; // function to restart level
    void openCharacterSelect();
    void openCombat();
    void openReward();

private:
    QGraphicsScene* gamescene;
    Level1* level_1 ;



protected:
    void keyPressEvent(QKeyEvent *event) override;
};


#endif // GAME_H
