#include "player.h"

Player::Player() {

    skin = QPixmap(":/images/Images/skin.png");

    int scaledWidth = skin.width() * 200 / skin.height();
    skin = skin.scaled(scaledWidth, 200, Qt::KeepAspectRatio);
    setPixmap(skin);
}
