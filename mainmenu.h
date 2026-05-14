#ifndef MAINMENU_H
#define MAINMENU_H

#include <QGraphicsScene>
#include "game.h"
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QGraphicsScene>
#include <QFontDatabase>
#include <QRandomGenerator>

class Mainmenu : public QGraphicsScene
{
    Q_OBJECT // needed for qt
public:
    Mainmenu(Game* game); // main menu takes game as a paramter
private:
    Game* game;
public slots:
    void new_game();
    void continue_game();
};

#endif // MAINMENU_H
