#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>

class Game : public QGraphicsView {
    Q_OBJECT //needed for qt
public:
    Game(int w, int h); // constructor with width and height paramters
    void closeMenu();
    void openMenu();
    void openLevel1();
private:
    QGraphicsScene* gameScene; // the scene for the game


};


#endif // GAME_H
