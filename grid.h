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

    class Trap // member class
    {

    };

private:
    QGraphicsScene* gamescene; // the scene for the game

    static const int rows = 20;
    static const int cols = 20;
    static const int tileSize = 30;

    vector<vector<int>> roomGrid;
};

#endif // GRID_H
