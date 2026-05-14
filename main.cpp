#include <QApplication>
#include <cstdlib>
#include <ctime>
#include "game.h"
#include "grid.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    srand(time(0));

    Game game(1280, 720);

    game.current_level = 5;
    game.setSelectedCharacter("mage") ;
    game.openReward();

    game.show();

    return a.exec();
}
