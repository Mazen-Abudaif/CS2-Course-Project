#include <QApplication>
#include <cstdlib>
#include <ctime>
#include "game.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    srand(time(0));

    Game game(1280, 720);

    game.show();


    return a.exec();
}
