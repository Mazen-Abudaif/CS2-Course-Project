#ifndef GRID_H
#define GRID_H
#include "boss.h"
#include "cardtype.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QWidget>
#include <vector>
#include <QPoint>
#include <QGraphicsRectItem>

using namespace std;

class Grid
{
public:
    Grid(QGraphicsScene* scene, int current_level = 1,bool spawnBoss = true) ; // the parametrized constructor

    // functions to spawn the items
    void SpawnTraps(int traps_no) ;
    void SpawnCards(CardType t , int number_of_cards) ;

    // function to create empty grid and determine tile type (wall or floor)
    void initialize_room() ;

    // function to draw wall and floor tiles
    void draw_room() ;

    // functions to draw the tiles in the grid
    void draw_walltile(int x, int y, int row, int col);
    void draw_floortile(int x, int y, int row, int col);

    // function to set trap in its place
    void setTrap(int traps_no, QGraphicsPixmapItem* trap) ;

    bool isWalkable(int row, int col);
    int get_tile_size() ;

    // getting player positions after offsetting the grid to make it appear in the middle
    pair<int,int> calcScenePosition(int row, int col);

    // function to place the boss randomly
    void Place_boss() ;

    // functions to place level 4 enemy types
    void PlaceFastEnemy() ;
    void PlaceTankyEnemy() ;

    //function to create detection circle
    void createDetectionCircle(QGraphicsScene* scene, Grid* room);

    bool isPlayerNearby(int playerRow, int playerCol);

    // returns stats of the enemy that triggered combat
    int getTriggeredCombatHp() const ;
    int getTriggeredImmuneTurns() const ;

    // level 4 enemy management
    void removeTriggeredEnemy() ;
    bool allEnemiesDefeated() const ;

    // level 5 special grid
    void createLevel5layout() ;

    // function to create card collectibles and place them
    void PlaceCards(int times,CardType type, const QPixmap& card) ;

    // vector to store row and col of traps, to be able to check if player stepped on them
    vector<pair<int,int>> trap_places;

    // vector to store row and col of cards, to be able to check if player stepped on them
    // also stores item for later removal
    vector<pair<pair<int,int>,QGraphicsPixmapItem*>> attack_card_places;
    vector<pair<pair<int,int>,QGraphicsPixmapItem*>> block_card_places;
    vector<pair<pair<int,int>,QGraphicsPixmapItem*>> heal_card_places;

    bool isCardPlaceTaken(int row,int col) ;

    // function to remove cards once stepped on them
    void RemoveCard(pair<int, int> place) ;

    // helper function to help place cards
    void PlaceCardAt(CardType type , int row, int col, const QPixmap& card) ;

    // visuals for level 5
    vector<vector<QGraphicsRectItem*>> darknessTiles;

    void createDarkness();
    void updateDarkness(int playerRow, int playerCol);

private:
    QGraphicsScene* gamescene; // the scene for the game
    int detectionRange ;
    Boss* boss ;
    Boss* fastEnemy ;
    Boss* tankyEnemy ;

    QGraphicsEllipseItem* detectionCircle ;
    QGraphicsEllipseItem* fastEnemyCircle ;
    QGraphicsEllipseItem* tankyEnemyCircle ;

    int triggeredCombatHp ;
    int triggeredImmuneTurns ;
    Boss* triggeredEnemy ;
    int current_level ;

    int rows = 17;
    int cols = 20;
    static const int tileSize = 30;

    vector<vector<int>> roomGrid;

    int offsetX;
    int offsetY;


};

#endif // GRID_H
