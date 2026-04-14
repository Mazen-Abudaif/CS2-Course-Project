#include "grid.h"
#include <QBrush>
#include <QPen>
#include <QColor>


Grid::Grid()
{

    initialize_room();
    draw_room() ;
    gamescene->setSceneRect(0, 0, cols * tileSize, rows * tileSize);
}

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

    gamescene->addRect(x, y, tileSize, tileSize,QPen(QColor(20,10,30)),QBrush(QColor(40, 28, 50)));
}



void Grid::draw_floortile(int x,int y, int row, int col)
{
    QColor floorBase(75, 72, 98);
    QColor gridLine(28, 18, 38);

    // slight variation so tiles don't all look identical
    // making a pattern
    int variation = ((row + col) % 3) * 6;

    gamescene->addRect(x, y, tileSize, tileSize,QPen(gridLine),QBrush(QColor(floorBase.red() + variation,floorBase.green()
                                                                                                                + variation / 2,floorBase.blue() + variation)));

}
