#include "player.h"

Player::Player() {

    skin = QPixmap(":/images/Images/skin.png");

    int scaledWidth = skin.width() * 200 / skin.height();
    skin = skin.scaled(50, 50, Qt::KeepAspectRatio);
    setPixmap(skin);

}
