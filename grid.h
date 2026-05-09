#ifndef GRID_H
#define GRID_H

#include "boss.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <vector>
#include <QPoint>


using namespace std;

class Grid
{
public:
    Grid(QGraphicsScene* scene, int traps_no , int attack_cards_no, int block_cards_no) ;; // the parametrized constructor

    // function to create empty grid and determine tile type (wall or floor)
    void initialize_room() ;

    // function to draw wall and floor tiles
    void draw_room() ;

    // functions to draw the tiles in the grid
    void draw_walltile(int x, int y, int row, int col);
    void draw_floortile(int x, int y, int row, int col);


    // function to set trap in its place
    void setTrap(int traps_no, QGraphicsPixmapItem* trap) ;

    // function to set trap places in terms of rows and columns
    void set_trap_places(int row, int col) ;

    bool isWalkable(int row, int col);
    int get_tile_size() ;

    // getting player positions after offsetting the grid to make it appear in the middle
    pair<int,int> calcScenePosition(int row, int col);

    // function to place the boss randomly
    void Place_boss(Boss* boss) ;

    //function to create detection circle
    void createDetectionCircle(QGraphicsScene* scene, Grid* room);

    bool isPlayerNearby(int playerRow, int playerCol);

    void updateBossDetection(int playerRow, int playerCol);

    // function to create card collectibles and place them
    void PlaceCards(int times, string type , QGraphicsPixmapItem* card) ;

    // vector to store row and col of traps, to be able to check if player stepped on them
    vector<pair<int,int>> trap_places;

    // vector to store row and col of cards, to be able to check if player stepped on them
    vector<pair<int,int>> attack_card_places;
    vector<pair<int,int>> block_card_places;

    bool isCardPlaceTaken(int row,int col) ;

private:
    QGraphicsScene* gamescene; // the scene for the game
    Boss *boss ;

    static const int rows = 15;
    static const int cols = 20;
    static const int tileSize = 30;

    vector<vector<int>> roomGrid;

    int detectionRange ;
    QGraphicsEllipseItem* detectionCircle;

    int offsetX;
    int offsetY;
};

#endif // GRID_H
