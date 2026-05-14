#include "player.h"
#include "consumable.h"

Player::Player(QString characterType) : Character(100) {

    if(characterType == "mage")
    {
        deck.append(new Consumable("Fireball",20));

        skin = QPixmap(":/images/Images/Mage.png");

        int scaledWidth = skin.width() * 200 / skin.height();
        skin = skin.scaled(50, 50, Qt::KeepAspectRatio);
        setPixmap(skin);
    
    } else if(characterType == "warrior")
    {
        deck.append(new Consumable("Shield",25));

        skin = QPixmap(":/images/Images/Warrior.png");

        int scaledWidth = skin.width() * 200 / skin.height();
        skin = skin.scaled(50, 50, Qt::KeepAspectRatio);
        setPixmap(skin);
    }
}
