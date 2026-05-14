#include "player.h"
#include "consumable.h"

Player::Player(QString characterType) : Character(100) {

    if(characterType == "Mage")
    {
        deck.append(new Consumable("Fireball",20));

        skin = QPixmap(":/images/Images/Mage.png");

        skin = skin.scaled(50, 50, Qt::KeepAspectRatio);
        setPixmap(skin);
    
    } else if(characterType == "Warrior")
    {
        deck.append(new Consumable("Shield",25));

        skin = QPixmap(":/images/Images/Warrior.png");

        skin = skin.scaled(50, 50, Qt::KeepAspectRatio);
        setPixmap(skin);
    }
}
