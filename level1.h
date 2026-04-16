#ifndef LEVEL1_H
#define LEVEL1_H
#include "baselevel.h"
#include "grid.h"

class Level1 : public Baselevel
{
public:
    Level1(QGraphicsScene* scene, Game* game);
    void initialise() override;
private:
};

#endif // LEVEL1_H
