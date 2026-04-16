#include "grid.h"
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QMovie>
#include <QGraphicsProxyWidget>


Grid::Grid(QGraphicsScene* scene)
    : gamescene(scene)
{
    int gridWidth = cols * tileSize;
    int gridHeight = rows * tileSize;

    offsetX = (1280 - gridWidth) / 2;
    offsetY = (720 - gridHeight) / 2;

    initialize_room();
    draw_room() ;
    gamescene->setSceneRect(0, 0, cols * tileSize, rows * tileSize);

    QPixmap trapPixmap(":/images/Images/trap.png");
    trapPixmap = trapPixmap.scaled(35, 35, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* trapItem = new QGraphicsPixmapItem(trapPixmap);
    QGraphicsPixmapItem* trapItem2 = new QGraphicsPixmapItem(trapPixmap);
    setTrap(trapItem);
    setTrap(trapItem2) ;


}

//function that checks if tile is walkable - for player movement
bool Grid:: isWalkable(int row, int col)
{
    // check if outside range
    if(row<0||col<0||row>=rows||col>=cols)
            return false ;

    //returns if its a wall or floor tile
    return roomGrid[row][col]==0 ;

}

int Grid::get_tile_size()
{
    return tileSize ;
}

void Grid::set_trap_places(int x, int y)
{
    trap_places.push_back({x,y}) ;
}

void Grid::setTrap(QGraphicsPixmapItem* trap){
    // setting the traps in random places each time
    //making sure trap is not in wall tiles
    int row, col;
    do{
            row = (rand()%rows) ;
            col = (rand()%cols) ;
    } while (row==0||col==0||row==rows-1||col==cols-1) ;

    // saving trap place
    set_trap_places(row,col) ;


    pair<int,int> pos = calcScenePosition(row, col);

    int tx = pos.first + (tileSize - trap->pixmap().width()) / 2;
    int ty = pos.second + (tileSize - trap->pixmap().height()) / 2;

    trap->setPos(tx, ty);

    gamescene->addItem(trap);
    trap->setZValue(1);

};


void Grid::initialize_room()
{
    // resize the room grid to a certain number of rows, and create empty columns for each row
    roomGrid.resize(rows,vector<int>(cols,0)) ;

    //loop over the rows
    for(int row=0 ; row<rows ; row++)
    {
        //loop over the columns
        for(int col=0 ; col<cols ; col++)
        {
            //get border tiles
            if(row==0||col==0||row==rows-1||col==cols-1)
            {
                roomGrid[row][col]= 1;
                // creates the wall
            }
            else
            {
                roomGrid[row][col]=0;
                // creates the floor
            }
        }
    }
}

void Grid::draw_room()
{
    //gamescene->setBackgroundBrush(QBrush(Qt::darkMagenta));
    //loop over the rows
    for(int row=0 ; row<rows ; row++)
    {
        //loop over columns
        for(int col=0 ; col<cols ; col++)
        {
            // making the grid using tileSize and number of col/rows
            int x = offsetX + col * tileSize;
            int y = offsetY + row * tileSize;

            if(roomGrid[row][col]==1)
            {
                draw_walltile(x,y,row,col) ;
            }
            else
            {

                draw_floortile(x,y,row,col) ;
            }
        }
    }
}

void Grid::draw_walltile(int x, int y, int row, int col)
{

        Q_UNUSED(row);
        Q_UNUSED(col);

        QColor wallColor(0,0,0);

        gamescene->addRect(x, y, tileSize, tileSize,
                           Qt::NoPen,
                           QBrush(wallColor));

}



void Grid::draw_floortile(int x,int y, int row, int col)
{

        Q_UNUSED(row);
        Q_UNUSED(col);

        QColor floorColor(15, 15, 28);
        QColor dotColor(225,0,0);

        // base floor tile
        gamescene->addRect(x, y, tileSize, tileSize,
                           Qt::NoPen,
                           QBrush(floorColor));

        // center dot
        int dotSize = 2;
        int dotX = x + (tileSize - dotSize) / 2;
        int dotY = y + (tileSize - dotSize) / 2;

        gamescene->addEllipse(dotX, dotY, dotSize, dotSize,
                              QPen(Qt::NoPen),
                              QBrush(dotColor));
}

// gets position of character on grid after offsetting.

pair<int,int> Grid::calcScenePosition(int row, int col)
{
    int x = offsetX + col * tileSize;
    int y = offsetY + row * tileSize;
    return {x,y} ;
}
