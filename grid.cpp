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
#include <ranlib.h>


Grid::Grid(QGraphicsScene* scene)
    : gamescene(scene)
{

    initialize_room();
    draw_room() ;
    gamescene->setSceneRect(0, 0, cols * tileSize, rows * tileSize);

    QPixmap trapPixmap(":/images/Images/trap.png");
    trapPixmap = trapPixmap.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* trapItem = new QGraphicsPixmapItem(trapPixmap);
    QGraphicsPixmapItem* trapItem2 = new QGraphicsPixmapItem(trapPixmap);
    setTrap(trapItem);
    setTrap(trapItem2) ;

}
void Grid::set_trap_places(int x, int y)
{
    trap_places[starting_index].first = x;
    trap_places[starting_index].second = y;
    starting_index++ ;
}

void Grid::setTrap(QGraphicsPixmapItem* trap){
    // setting the traps in random places each time
    //making sure trap is not in wall tiles
    int row, col;
    do{
            row = (arc4random()%rows) ;
            col = (arc4random()%cols) ;
    } while (row==0||col==0||row==rows-1||col==cols-1) ;


    int x_cor = col * tileSize;
    int y_cor = row * tileSize;

    gamescene->addItem(trap);
    trap->setPos(x_cor,y_cor);
    trap->setZValue(1);

    set_trap_places(x_cor,y_cor) ;
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
            int x = col*tileSize ;
            int y = row*tileSize ;

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

        QColor wallColor(48, 25, 52);

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
