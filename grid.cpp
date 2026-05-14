#include "grid.h"
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QGridLayout>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>


Grid::Grid(QGraphicsScene* scene, bool spawnBoss, bool dense, bool level3Mode)
    : gamescene(scene),
    dense(dense),
    level3Mode(level3Mode),
    boss(nullptr),
    fastEnemy(nullptr),
    tankyEnemy(nullptr),
    detectionCircle(nullptr),
    fastEnemyCircle(nullptr),
    tankyEnemyCircle(nullptr),
    level3BossCircle(nullptr),
    triggeredCombatHp(60),
    triggeredImmuneTurns(0),
    triggeredEnemy(nullptr),
    doorRow(7),
    doorCol(13),
    doorItem(nullptr),
    keyItem(nullptr),
    keyDropped(false),
    playerHasKey(false)
{
    int gridWidth = cols * tileSize;
    int gridHeight = rows * tileSize;

    offsetX = (1280 - gridWidth) / 2;
    offsetY = (720 - gridHeight) / 2;

    initialize_room();
    draw_room() ;
    gamescene->setSceneRect(0, 0, cols * tileSize, rows * tileSize);

    if (!level3Mode)
    {
        if (spawnBoss)
            Place_boss() ;
        else
        {
            PlaceFastEnemy() ;
            PlaceTankyEnemy() ;
        }
    }

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

    if(roomGrid[row][col]==1)
        return false ;

    // door tile is blocked until player has the key
    if(level3Mode && !playerHasKey && row == doorRow && col == doorCol)
        return false ;

    return true ;

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
            row = (rand()%rows) ;
            col = (rand()%cols) ;
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

    if (boss != nullptr)
    {
        int rowDiff = row - boss->getRow() ;
        int colDiff = col - boss->getCol() ;
        if ((rowDiff*rowDiff + colDiff*colDiff) <= (detectionRange*detectionRange))
            return true ;
    }

    if (fastEnemy != nullptr)
    {
        int rowDiff = row - fastEnemy->getRow() ;
        int colDiff = col - fastEnemy->getCol() ;
        if ((rowDiff*rowDiff + colDiff*colDiff) <= (detectionRange*detectionRange))
            return true ;
    }

    if (tankyEnemy != nullptr)
    {
        int rowDiff = row - tankyEnemy->getRow() ;
        int colDiff = col - tankyEnemy->getCol() ;
        if ((rowDiff*rowDiff + colDiff*colDiff) <= (detectionRange*detectionRange))
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

void Grid::PlaceFastEnemy()
{
    fastEnemy = new Boss(40, BossType::Fast) ;

    int row = rows / 4 ;
    int col = 3 * cols / 4 ;

    fastEnemy->setGridPosition(row, col) ;

    pair<int,int> pos = calcScenePosition(row, col) ;

    QPixmap FastPixmap(":/images/Images/demogorgon (enemy).png") ;
    FastPixmap = FastPixmap.scaled(35, 35, Qt::KeepAspectRatio, Qt::SmoothTransformation) ;
    fastEnemy->setPixmap(FastPixmap) ;

    int tx = pos.first + (tileSize - fastEnemy->pixmap().width()) / 2 ;
    int ty = pos.second + (tileSize - fastEnemy->pixmap().height()) / 2 ;

    fastEnemy->setPos(tx, ty) ;
    gamescene->addItem(fastEnemy) ;
    fastEnemy->setZValue(2) ;

    int radius = detectionRange * tileSize ;
    int centerX = pos.first + tileSize / 2 ;
    int centerY = pos.second + tileSize / 2 ;

    fastEnemyCircle = new QGraphicsEllipseItem(centerX - radius, centerY - radius, radius * 2, radius * 2) ;
    fastEnemyCircle->setBrush(QColor(0, 150, 255, 40)) ; // blue tint for fast enemy
    fastEnemyCircle->setPen(Qt::NoPen) ;
    fastEnemyCircle->setZValue(1) ;
    gamescene->addItem(fastEnemyCircle) ;
}

void Grid::PlaceTankyEnemy()
{
    tankyEnemy = new Boss(120, BossType::Tanky) ;

    int row = 3 * rows / 4 ;
    int col = cols / 4 ;

    tankyEnemy->setGridPosition(row, col) ;

    pair<int,int> pos = calcScenePosition(row, col) ;

    QPixmap TankyPixmap(":/images/Images/demogorgon (enemy).png") ;
    TankyPixmap = TankyPixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation) ;
    tankyEnemy->setPixmap(TankyPixmap) ;

    int tx = pos.first + (tileSize - tankyEnemy->pixmap().width()) / 2 ;
    int ty = pos.second + (tileSize - tankyEnemy->pixmap().height()) / 2 ;

    tankyEnemy->setPos(tx, ty) ;
    gamescene->addItem(tankyEnemy) ;
    tankyEnemy->setZValue(2) ;

    int radius = detectionRange * tileSize ;
    int centerX = pos.first + tileSize / 2 ;
    int centerY = pos.second + tileSize / 2 ;

    tankyEnemyCircle = new QGraphicsEllipseItem(centerX - radius, centerY - radius, radius * 2, radius * 2) ;
    tankyEnemyCircle->setBrush(QColor(255, 100, 0, 40)) ; // orange tint for tanky enemy
    tankyEnemyCircle->setPen(Qt::NoPen) ;
    tankyEnemyCircle->setZValue(1) ;
    gamescene->addItem(tankyEnemyCircle) ;
}

void Grid::PlaceCards(int times,CardType type, QGraphicsPixmapItem* card)
{
    for(int i=0 ; i<times ; i++)
    {
    int row, col;
    do{
        row = (rand()%rows) ;
        col = (rand()%cols) ;
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
    if(dense)
    {
        // horizontal wall, row 4, cols 2–7 (gap at col 8 so player can pass)
        for (int col = 2; col <= 7; col++)
            roomGrid[4][col] = 1;

        // horizontal wall, row 10, cols 12–17 (gap at col 11)
        for (int col = 12; col <= 17; col++)
            roomGrid[10][col] = 1;

        // vertical wall, col 10, rows 5–9 (gap at row 4 and row 10)
        for (int row = 5; row <= 9; row++)
            roomGrid[row][10] = 1;
    }

    if(level3Mode)
    {
        // Vertical dividing wall creating the boss room on the right side.
        // Row 7 (doorRow) is left open as a floor tile; the door mechanic
        // blocks it in isWalkable() until the player has the key.
        for (int r = 1; r <= rows-2; r++)
        {
            if (r != doorRow)
                roomGrid[r][doorCol] = 1;
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

    QGraphicsEllipseItem* detectionCircle = new QGraphicsEllipseItem(
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

void Grid::PlaceLevel2Enemy1()
{
    fastEnemy = new Boss(80, BossType::Regular, 65) ;

    int row = rows / 4 ;
    int col = cols / 4 ;

    fastEnemy->setGridPosition(row, col) ;
    pair<int,int> pos = calcScenePosition(row, col) ;

    QPixmap pix(":/images/Images/demogorgon (enemy).png") ;
    pix = pix.scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation) ;
    fastEnemy->setPixmap(pix) ;

    int tx = pos.first + (tileSize - fastEnemy->pixmap().width()) / 2 ;
    int ty = pos.second + (tileSize - fastEnemy->pixmap().height()) / 2 ;
    fastEnemy->setPos(tx, ty) ;
    gamescene->addItem(fastEnemy) ;
    fastEnemy->setZValue(2) ;

    int radius = detectionRange * tileSize ;
    int centerX = pos.first + tileSize / 2 ;
    int centerY = pos.second + tileSize / 2 ;
    fastEnemyCircle = new QGraphicsEllipseItem(centerX - radius, centerY - radius, radius * 2, radius * 2) ;
    fastEnemyCircle->setBrush(QColor(200, 0, 200, 40)) ;
    fastEnemyCircle->setPen(Qt::NoPen) ;
    fastEnemyCircle->setZValue(1) ;
    gamescene->addItem(fastEnemyCircle) ;
}

void Grid::PlaceLevel2Enemy2()
{
    tankyEnemy = new Boss(80, BossType::Regular, 65) ;

    int row = 3 * rows / 4 ;
    int col = 3 * cols / 4 ;

    tankyEnemy->setGridPosition(row, col) ;
    pair<int,int> pos = calcScenePosition(row, col) ;

    QPixmap pix(":/images/Images/demogorgon (enemy).png") ;
    pix = pix.scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation) ;
    tankyEnemy->setPixmap(pix) ;

    int tx = pos.first + (tileSize - tankyEnemy->pixmap().width()) / 2 ;
    int ty = pos.second + (tileSize - tankyEnemy->pixmap().height()) / 2 ;
    tankyEnemy->setPos(tx, ty) ;
    gamescene->addItem(tankyEnemy) ;
    tankyEnemy->setZValue(2) ;

    int radius = detectionRange * tileSize ;
    int centerX = pos.first + tileSize / 2 ;
    int centerY = pos.second + tileSize / 2 ;
    tankyEnemyCircle = new QGraphicsEllipseItem(centerX - radius, centerY - radius, radius * 2, radius * 2) ;
    tankyEnemyCircle->setBrush(QColor(200, 0, 200, 40)) ;
    tankyEnemyCircle->setPen(Qt::NoPen) ;
    tankyEnemyCircle->setZValue(1) ;
    gamescene->addItem(tankyEnemyCircle) ;
}

bool Grid::isPlayerNearby(int playerRow, int playerCol)
{
    if (boss != nullptr)
    {
        int dist = abs(playerRow - boss->getRow()) + abs(playerCol - boss->getCol()) ;
        if (dist <= detectionRange)
        {
            // In level 3 the boss room is locked until the player holds the key
            if (level3Mode && !playerHasKey)
                return false ;

            triggeredEnemy = boss ;
            triggeredCombatHp = level3Mode ? boss->getHealth() : 60 ;
            triggeredImmuneTurns = 0 ;
            return true ;
        }
    }

    if (fastEnemy != nullptr)
    {
        int dist = abs(playerRow - fastEnemy->getRow()) + abs(playerCol - fastEnemy->getCol()) ;
        if (dist <= detectionRange)
        {
            triggeredEnemy = fastEnemy ;
            triggeredCombatHp = fastEnemy->getHealth();
            triggeredImmuneTurns = 0 ;
            return true ;
        }
    }

    if (tankyEnemy != nullptr)
    {
        int dist = abs(playerRow - tankyEnemy->getRow()) + abs(playerCol - tankyEnemy->getCol()) ;
        if (dist <= detectionRange)
        {
            triggeredEnemy = tankyEnemy ;
            triggeredCombatHp = tankyEnemy->getHealth();
            triggeredImmuneTurns = 2 ;
            return true ;
        }
    }

    return false ;
}

int Grid::getTriggeredCombatHp() const
{
    return triggeredCombatHp ;
}

int Grid::getTriggeredImmuneTurns() const
{
    return triggeredImmuneTurns ;
}

void Grid::removeTriggeredEnemy()
{
    if (triggeredEnemy == nullptr)
        return ;

    if (triggeredEnemy == fastEnemy)
    {
        // In level 3, Guard 1 drops the key when defeated
        if (level3Mode)
            spawnKey(fastEnemy->getRow(), fastEnemy->getCol()) ;

        gamescene->removeItem(fastEnemy) ;
        delete fastEnemy ;
        fastEnemy = nullptr ;

        if (fastEnemyCircle != nullptr)
        {
            gamescene->removeItem(fastEnemyCircle) ;
            delete fastEnemyCircle ;
            fastEnemyCircle = nullptr ;
        }
    }
    else if (triggeredEnemy == tankyEnemy)
    {
        gamescene->removeItem(tankyEnemy) ;
        delete tankyEnemy ;
        tankyEnemy = nullptr ;

        if (tankyEnemyCircle != nullptr)
        {
            gamescene->removeItem(tankyEnemyCircle) ;
            delete tankyEnemyCircle ;
            tankyEnemyCircle = nullptr ;
        }
    }
    else if (level3Mode && triggeredEnemy == boss)
    {
        gamescene->removeItem(boss) ;
        delete boss ;
        boss = nullptr ;

        if (level3BossCircle != nullptr)
        {
            gamescene->removeItem(level3BossCircle) ;
            delete level3BossCircle ;
            level3BossCircle = nullptr ;
        }
    }

    triggeredEnemy = nullptr ;
}

bool Grid::allEnemiesDefeated() const
{
    if (level3Mode)
        return fastEnemy == nullptr && tankyEnemy == nullptr && boss == nullptr ;
    return fastEnemy == nullptr && tankyEnemy == nullptr ;
}

// ── Level 3 ──────────────────────────────────────────────────────────────────

void Grid::PlaceLevel3Guard1()
{
    // Guard 1: regular enemy that drops the key when defeated
    fastEnemy = new Boss(70, BossType::Regular, 55) ;

    int row = rows / 4 ;        // row 3
    int col = cols / 4 ;        // col 5

    fastEnemy->setGridPosition(row, col) ;
    pair<int,int> pos = calcScenePosition(row, col) ;

    QPixmap pix(":/images/Images/demogorgon (enemy).png") ;
    pix = pix.scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation) ;
    fastEnemy->setPixmap(pix) ;

    int tx = pos.first  + (tileSize - fastEnemy->pixmap().width())  / 2 ;
    int ty = pos.second + (tileSize - fastEnemy->pixmap().height()) / 2 ;
    fastEnemy->setPos(tx, ty) ;
    gamescene->addItem(fastEnemy) ;
    fastEnemy->setZValue(2) ;

    int radius  = detectionRange * tileSize ;
    int centerX = pos.first  + tileSize / 2 ;
    int centerY = pos.second + tileSize / 2 ;

    fastEnemyCircle = new QGraphicsEllipseItem(centerX - radius, centerY - radius, radius * 2, radius * 2) ;
    fastEnemyCircle->setBrush(QColor(0, 200, 100, 40)) ;   // green tint – key dropper
    fastEnemyCircle->setPen(Qt::NoPen) ;
    fastEnemyCircle->setZValue(1) ;
    gamescene->addItem(fastEnemyCircle) ;
}

void Grid::PlaceLevel3Guard2()
{
    // Guard 2: standard guard, no key drop
    tankyEnemy = new Boss(70, BossType::Regular, 55) ;

    int row = 3 * rows / 4 ;   // row 11
    int col = cols / 4 ;        // col 5

    tankyEnemy->setGridPosition(row, col) ;
    pair<int,int> pos = calcScenePosition(row, col) ;

    QPixmap pix(":/images/Images/demogorgon (enemy).png") ;
    pix = pix.scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation) ;
    tankyEnemy->setPixmap(pix) ;

    int tx = pos.first  + (tileSize - tankyEnemy->pixmap().width())  / 2 ;
    int ty = pos.second + (tileSize - tankyEnemy->pixmap().height()) / 2 ;
    tankyEnemy->setPos(tx, ty) ;
    gamescene->addItem(tankyEnemy) ;
    tankyEnemy->setZValue(2) ;

    int radius  = detectionRange * tileSize ;
    int centerX = pos.first  + tileSize / 2 ;
    int centerY = pos.second + tileSize / 2 ;

    tankyEnemyCircle = new QGraphicsEllipseItem(centerX - radius, centerY - radius, radius * 2, radius * 2) ;
    tankyEnemyCircle->setBrush(QColor(200, 100, 0, 40)) ;   // amber tint
    tankyEnemyCircle->setPen(Qt::NoPen) ;
    tankyEnemyCircle->setZValue(1) ;
    gamescene->addItem(tankyEnemyCircle) ;
}

void Grid::PlaceLevel3LockedBoss()
{
    // Boss: 100 HP, sits inside the locked room
    boss = new Boss(100, BossType::Regular, 70) ;

    int row = rows / 2 ;        // row 7  (centre of boss room)
    int col = (doorCol + cols - 1) / 2 ;   // col ≈ 16

    boss->setGridPosition(row, col) ;
    pair<int,int> pos = calcScenePosition(row, col) ;

    QPixmap pix(":/images/Images/demogorgon (enemy).png") ;
    pix = pix.scaled(55, 55, Qt::KeepAspectRatio, Qt::SmoothTransformation) ;
    boss->setPixmap(pix) ;

    int tx = pos.first  + (tileSize - boss->pixmap().width())  / 2 ;
    int ty = pos.second + (tileSize - boss->pixmap().height()) / 2 ;
    boss->setPos(tx, ty) ;
    gamescene->addItem(boss) ;
    boss->setZValue(2) ;

    int radius  = detectionRange * tileSize ;
    int centerX = pos.first  + tileSize / 2 ;
    int centerY = pos.second + tileSize / 2 ;

    level3BossCircle = new QGraphicsEllipseItem(centerX - radius, centerY - radius, radius * 2, radius * 2) ;
    level3BossCircle->setBrush(QColor(255, 0, 0, 40)) ;   // red tint
    level3BossCircle->setPen(Qt::NoPen) ;
    level3BossCircle->setZValue(1) ;
    gamescene->addItem(level3BossCircle) ;

    // Locked door visual: golden rectangle over the door floor tile
    pair<int,int> doorPos = calcScenePosition(doorRow, doorCol) ;
    doorItem = new QGraphicsRectItem(doorPos.first, doorPos.second, tileSize, tileSize) ;
    doorItem->setBrush(QColor(218, 165, 32)) ;             // gold
    doorItem->setPen(QPen(QColor(139, 100, 0), 2)) ;
    doorItem->setZValue(3) ;
    gamescene->addItem(doorItem) ;
}

void Grid::spawnKey(int row, int col)
{
    key_place  = {row, col} ;
    keyDropped = true ;

    pair<int,int> pos = calcScenePosition(row, col) ;
    int cx = pos.first  + tileSize / 2 ;
    int cy = pos.second + tileSize / 2 ;
    int r  = 8 ;

    keyItem = new QGraphicsEllipseItem(cx - r, cy - r, r * 2, r * 2) ;
    keyItem->setBrush(QColor(255, 215, 0)) ;   // bright gold
    keyItem->setPen(QPen(QColor(180, 140, 0), 2)) ;
    keyItem->setZValue(3) ;
    gamescene->addItem(keyItem) ;
}

bool Grid::isKeyAt(int row, int col) const
{
    return keyDropped && !playerHasKey && key_place.first == row && key_place.second == col ;
}

void Grid::pickUpKey()
{
    playerHasKey = true ;

    if (keyItem != nullptr)
    {
        gamescene->removeItem(keyItem) ;
        delete keyItem ;
        keyItem = nullptr ;
    }

    // Remove the locked door visual; isWalkable() will now allow passage
    if (doorItem != nullptr)
    {
        gamescene->removeItem(doorItem) ;
        delete doorItem ;
        doorItem = nullptr ;
    }
}

bool Grid::getPlayerHasKey() const
{
    return playerHasKey ;
}

// ─────────────────────────────────────────────────────────────────────────────

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
