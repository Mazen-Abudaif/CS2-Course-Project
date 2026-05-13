#ifndef LEVEL4_H
#define LEVEL4_H
#include "baselevel.h"
#include "grid.h"

class Level4 : public Baselevel
{
public:
    Level4(QGraphicsScene* scene, Game* game);
    void initialise() override;

protected:
    void setupRoom() override;
};

#endif // LEVEL4_H
