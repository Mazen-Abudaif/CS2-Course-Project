#include <QApplication>
#include "game.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Game game(1280, 720);

    game.show();


    return a.exec();
}
