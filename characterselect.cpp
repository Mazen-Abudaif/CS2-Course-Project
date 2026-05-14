#include "characterselect.h"
#include <QGraphicsPixmapItem>
#include <QPushButton>

Characterselect::Characterselect(Game* game) : QGraphicsScene()
{
    this->game = game;
    QPixmap background(":/images/Images/background.jpg");
    background = background.scaled(1280,720, Qt::KeepAspectRatioByExpanding);
    QGraphicsPixmapItem* backgroundItem = new QGraphicsPixmapItem(background);
    addItem(backgroundItem);
    backgroundItem->setZValue(-1);


    // Mage sprite above button
    QPixmap magePixmap(":/images/Images/Mage.png");
    magePixmap = magePixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* mageItem = new QGraphicsPixmapItem(magePixmap);
    mageItem->setPos(330, 180);
    addItem(mageItem);

    // Warrior sprite above button
    QPixmap warriorPixmap(":/images/Images/Warrior.png");
    warriorPixmap = warriorPixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* warriorItem = new QGraphicsPixmapItem(warriorPixmap);
    warriorItem->setPos(750, 180);
    addItem(warriorItem);

    QPushButton* selectMage_Button = new QPushButton();
    selectMage_Button->setText("Mage");
    selectMage_Button->setObjectName(QString("selectMage_Button"));
    selectMage_Button->setToolTip("Mage");
    selectMage_Button->setGeometry(QRect(380, 400, 100, 32));
    selectMage_Button->setAutoFillBackground(false);
    connect(selectMage_Button, &QPushButton::clicked, this, &Characterselect::selectMage);
    addWidget(selectMage_Button);

    QPushButton* selectWarrior_Button = new QPushButton();
    selectWarrior_Button->setText("Warrior");
    selectWarrior_Button->setObjectName(QString("selectWarrior_Button"));
    selectWarrior_Button->setToolTip("Warrior");
    selectWarrior_Button->setGeometry(QRect(800, 400, 100, 32));
    selectWarrior_Button->setAutoFillBackground(false);
    connect(selectWarrior_Button, &QPushButton::clicked, this, &Characterselect::selectWarrior);
    addWidget(selectWarrior_Button);
}

void Characterselect::selectMage()
{
    game->setSelectedCharacter("mage");
    game->openLevel1();
}

void Characterselect::selectWarrior()
{
    game->setSelectedCharacter("warrior");
    game->openLevel1();
}

