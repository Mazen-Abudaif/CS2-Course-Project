#ifndef LEVEL2_H
#define LEVEL2_H
#include "baselevel.h"
#include "grid.h"

class Level2 : public Baselevel
{
public:
    Level2(QGraphicsScene* scene, Game* game);
    void initialise() override;

protected:
    void setupRoom() override;

};

#endif // LEVEL2_H
