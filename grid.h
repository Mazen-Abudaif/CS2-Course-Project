#ifndef GRID_H
#define GRID_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <vector>

using namespace std;

class Grid
{
public:
    Grid(QGraphicsScene* scene) ;; // the parametrized constructor
    void initialize_room() ;
    void draw_room() ;

    void draw_walltile(int x, int y, int row, int col);
    void draw_floortile(int x, int y, int row, int col);


    void setTrap(QGraphicsPixmapItem* trap) ;
    void set_trap_places(int x, int y) ;

private:
    QGraphicsScene* gamescene; // the scene for the game

    static const int rows = 20;
    static const int cols = 20;
    static const int tileSize = 30;

    int starting_index = 0;

    vector<vector<int>> roomGrid;
    vector<pair<int,int>> trap_places;
};

#endif // GRID_H
