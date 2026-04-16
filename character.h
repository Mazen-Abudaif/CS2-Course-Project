#ifndef CHARACTER_H
#define CHARACTER_H
#include <QGraphicsPixmapItem>
#include <QPixmap>

class Character : public QGraphicsPixmapItem
{
public:
    Character();

    void setGridPosition(int newRow, int newCol);
    void setScenePosition(int x, int y) ;
    int getRow() const;
    int getCol() const;

protected:
    int row;
    int col ;
    QPixmap skin;
};

#endif // CHARACTER_H
