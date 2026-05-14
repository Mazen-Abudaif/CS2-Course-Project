#ifndef LEVEL3_H
#define LEVEL3_H
#include "baselevel.h"
#include "grid.h"

class Level3 : public Baselevel
{
public:
    Level3(QGraphicsScene* scene, Game* game);
    void initialise() override;

protected:
    void setupRoom() override;
};

#endif // LEVEL3_H
