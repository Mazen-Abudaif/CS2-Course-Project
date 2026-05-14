#ifndef LEVEL5_H
#define LEVEL5_H
#include "baselevel.h"

class Level5 : public Baselevel
{
public:
    Level5(QGraphicsScene* scene, Game* game);
    void initialise() override;
private:
};


#endif // LEVEL5_H
