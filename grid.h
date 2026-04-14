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
    Grid(); // the default constructor
    void initialize_room() ;
    void draw_room() ;

    void draw_walltile(int x, int y, int row, int col);
    void draw_floortile(int x, int y, int row, int col);

private:
    QGraphicsScene* gamescene; // the scene for the game

    static const int rows = 15;
    static const int cols = 15;
    static const int tileSize = 40;

    vector<vector<int>> roomGrid;
};

#endif // GRID_H
