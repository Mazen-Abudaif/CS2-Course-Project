#ifndef GRID_H
#define GRID_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <vector>
#include <QPoint>


using namespace std;

class Grid
{
public:
    Grid(QGraphicsScene* scene) ;; // the parametrized constructor

    // function to create empty grid and determine tile type (wall or floor)
    void initialize_room() ;

    // function to draw wall and floor tiles
    void draw_room() ;

    // functions to draw the tiles in the grid
    void draw_walltile(int x, int y, int row, int col);
    void draw_floortile(int x, int y, int row, int col);


    // function to set trap in its place
    void setTrap(QGraphicsPixmapItem* trap) ;

    // function to sett trap places in terms of rows and columns
    void set_trap_places(int row, int col) ;

    bool isWalkable(int row, int col);
    int get_tile_size() ;

    // getting player positions after offsetting the grid to make it appear in the middle
    pair<int,int> calcScenePosition(int row, int col);

    // vector to store row and col of traps, to be able to check if player stepped on them
    vector<pair<int,int>> trap_places;

private:
    QGraphicsScene* gamescene; // the scene for the game

    static const int rows = 15;
    static const int cols = 20;
    static const int tileSize = 30;

    vector<vector<int>> roomGrid;

    int offsetX;
    int offsetY;
};

#endif // GRID_H
