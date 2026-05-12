#include "grid.h"
#include "boss.h"
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QGridLayout>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>


Grid::Grid(QGraphicsScene* scene)
    : gamescene(scene)
{
    detectionRange = 3 ; // 3 tiles

    int gridWidth = cols * tileSize;
    int gridHeight = rows * tileSize;

    offsetX = (1280 - gridWidth) / 2;
    offsetY = (720 - gridHeight) / 2;

    initialize_room();
    draw_room() ;
    gamescene->setSceneRect(0, 0, cols * tileSize, rows * tileSize);
    Place_boss() ;
    SpawnTraps(1) ;
    SpawnCards(CardType::Attack , 1) ;
    SpawnCards(CardType::Block , 1) ;
}

void Grid::SpawnTraps(int traps_no)
{
    QPixmap trapPixmap(":/images/Images/trap.png");
    trapPixmap = trapPixmap.scaled(35, 35, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* trapItem = new QGraphicsPixmapItem(trapPixmap);
    setTrap(traps_no,trapItem);
}

void Grid::SpawnCards(CardType type , int number_of_cards)
{
    if(type==CardType::Attack)
    {
        QPixmap Attack_Card(":/images/Images/Attack_Card.png") ;
        Attack_Card = Attack_Card.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsPixmapItem* AttackCardItem = new QGraphicsPixmapItem(Attack_Card);
        PlaceCards(number_of_cards,CardType::Attack,AttackCardItem) ;
    }

    if(type==CardType::Block)
    {
        QPixmap Block_Card(":/images/Images/Block_Card.png") ;
        Block_Card = Block_Card.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsPixmapItem* BlockCardItem = new QGraphicsPixmapItem(Block_Card);
        PlaceCards(number_of_cards,CardType::Block,BlockCardItem) ;
    }
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

void Grid::setTrap(int traps_no, QGraphicsPixmapItem* trap){
    // setting the traps in random places each time
    //making sure trap is not in wall tiles
    for(int i=0; i<traps_no ; i++)
    {

    int row, col;
    do{
            row = (arc4random()%rows) ;
            col = (arc4random()%cols) ;
    } while (row==0||col==0||row==rows-1||col==cols-1 || isCardPlaceTaken(row,col)==true) ;

    // saving trap place
    trap_places.push_back({row,col}) ;
    pair<int,int> pos = calcScenePosition(row, col);

    QGraphicsPixmapItem *newTrap = new QGraphicsPixmapItem(trap->pixmap());
    int tx,ty;

     tx = pos.first + (tileSize - newTrap->pixmap().width()) / 2;
     ty = pos.second + (tileSize - newTrap->pixmap().height()) / 2;

    newTrap->setPos(tx, ty);

    gamescene->addItem(newTrap);
    newTrap->setZValue(1);
    }
};

bool Grid::isCardPlaceTaken(int row,int col)
{

    size_t i=0 ;

    while(i<block_card_places.size())
    {
        if (row==block_card_places.at(i).first.first && col==block_card_places.at(i).first.second)
            return true;

        i++ ;
    }
    i=0;
    while(i<attack_card_places.size())
    {
        if (row==attack_card_places.at(i).first.first && col==attack_card_places.at(i).first.second)
            return true;

        i++ ;
    }
    i=0;
    while(i<trap_places.size())
    {
        if (row==trap_places.at(i).first && col==trap_places.at(i).first)
            return true;

        i++ ;
    }

    int rowDiff = row - boss->getRow() ;
    int colDiff = col - boss->getCol() ;

    if((rowDiff*rowDiff + colDiff*colDiff) <= (detectionRange*detectionRange))
    {
        return true ;
    }

    return false;
}

void Grid::Place_boss()
{
    boss = new Boss(100);

    if(boss==nullptr)
        return ;

    int row = rows/2 ;
    int col = cols/2 ;

    boss->setGridPosition(row, col);

    pair<int,int> pos = calcScenePosition(row, col);

    QPixmap BossPixmap(":/images/Images/demogorgon (enemy).png");
    BossPixmap = BossPixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    boss->setPixmap(BossPixmap) ;

    int tx = pos.first + (tileSize - boss->pixmap().width()) / 2;
    int ty = pos.second + (tileSize - boss->pixmap().height()) / 2;

    boss->setPos(tx, ty);

    gamescene->addItem(boss);
    boss->setZValue(2);
    createDetectionCircle(gamescene,this) ;
}

void Grid::PlaceCards(int times,CardType type, QGraphicsPixmapItem* card)
{
    for(int i=0 ; i<times ; i++)
    {
    int row, col;
    do{
        row = (arc4random()%rows) ;
        col = (arc4random()%cols) ;
    } while (row==0||col==0||row==rows-1||col==cols-1 || isCardPlaceTaken(row,col)==true) ;

        pair<int,int> pos ;
        if(type== CardType::Attack)
        {
            pos = calcScenePosition(row,col) ;
        }
        if(type == CardType::Block)
        {
            pos = calcScenePosition(row,col) ;
        }

        QGraphicsPixmapItem *newCard = new QGraphicsPixmapItem(card->pixmap());
        int x,y;
        x = pos.first + (tileSize - newCard->pixmap().width()) / 2;
        y = pos.second + (tileSize - newCard->pixmap().height()) / 2;

        newCard->setPos(x,y) ;

        gamescene->addItem(newCard) ;
        newCard->setZValue(1) ;

        if(type== CardType::Attack)
        {
            attack_card_places.push_back(pair(pair(row,col),newCard)) ;
        }
        else if(type == CardType::Block)
        {
            block_card_places.push_back(pair(pair(row,col),newCard)) ;
        }
        else
        {
            return ;
        }
    }
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

void Grid::createDetectionCircle(QGraphicsScene* scene, Grid* room)
{
    int tileSize = room->get_tile_size();

    int radius = detectionRange * tileSize;

    pair<int,int> pos = room->calcScenePosition(boss->getRow(), boss-> getCol());

    int centerX = pos.first + tileSize / 2;
    int centerY = pos.second + tileSize / 2;

    detectionCircle = new QGraphicsEllipseItem(
        centerX - radius,
        centerY - radius,
        radius * 2,
        radius * 2
        );

    detectionCircle->setBrush(QColor(255, 0, 0, 40)); // transparent red
    detectionCircle->setPen(Qt::NoPen);
    detectionCircle->setZValue(1); // behind boss

    scene->addItem(detectionCircle);
}

bool Grid:: isPlayerNearby(int playerRow, int playerCol)
{
    int dist = abs(playerRow - boss->getRow()) + abs(playerCol - boss->getCol() ) ;

    return dist<= detectionRange ;
}

void Grid::RemoveCard(pair<int, int> place)
{
    for(size_t i=0 ; i<attack_card_places.size(); i++)
    {
        if(place==attack_card_places.at(i).first)
        {
            gamescene->removeItem(attack_card_places.at(i).second) ;
            delete attack_card_places.at(i).second ;
            attack_card_places.erase(attack_card_places.begin()+i) ;
            return ;
        }
    }
    for(size_t i=0 ; i<block_card_places.size(); i++)
    {
        if(place==block_card_places.at(i).first)
        {
            gamescene->removeItem(block_card_places.at(i).second) ;
            delete block_card_places.at(i).second ;
            block_card_places.erase(block_card_places.begin()+i) ;
            return ;
        }
    }
}




