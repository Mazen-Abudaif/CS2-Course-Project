#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include "grid.h"

class Game : public QGraphicsView {
    Q_OBJECT //needed for qt
public:
    Game(int w, int h); // constructor with width and height paramters
    void closeMenu();
    void openMenu();
    void openLevel1();
private:
    QGraphicsScene* gamescene; // the scene for the game
    Grid *grid ;

};


#endif // GAME_H
