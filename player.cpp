#include "player.h"
#include "attackcard.h"
#include "blockcard.h"
#include "consumable.h"

Player::Player(QString characterType) : Character(100) {

    if(characterType == "mage")
    {
        deck.append(new Attackcard(10));
        deck.append(new Blockcard(5));
        deck.append(new Consumable("Fireball",20));

        skin = QPixmap(":/images/Images/skin.png");

        int scaledWidth = skin.width() * 200 / skin.height();
        skin = skin.scaled(scaledWidth, 200, Qt::KeepAspectRatio);
        setPixmap(skin);
    } else if(characterType == "warrior")
    {
        deck.append(new Attackcard(5));
        deck.append(new Blockcard(20));
        deck.append(new Consumable("Shield",25));

        skin = QPixmap(":/images/Images/skin.png");

        int scaledWidth = skin.width() * 200 / skin.height();
        skin = skin.scaled(scaledWidth, 200, Qt::KeepAspectRatio);
        setPixmap(skin);
    }
}
