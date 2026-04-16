#include "characterselect.h"
#include <QGraphicsPixmapItem>
#include <QPushButton>

Characterselect::Characterselect(Game* game) : QGraphicsScene()
{
    this->game = game;
    QPixmap background(":/images/startscreen.png");
    background = background.scaled(1280,720, Qt::KeepAspectRatioByExpanding);
    QGraphicsPixmapItem* backgroundItem = new QGraphicsPixmapItem(background);
    addItem(backgroundItem);
    backgroundItem->setZValue(-1);

    QPushButton* selectMage_Button = new QPushButton();
    selectMage_Button->setText("Mage");
    selectMage_Button->setObjectName(QString("selectMage_Button"));
    selectMage_Button->setToolTip("Mage");
    selectMage_Button->setGeometry(QRect(490, 400, 100, 32)); // puts button at coords 640,360 with width/height 100/32
    selectMage_Button->setAutoFillBackground(false);
    connect(selectMage_Button, &QPushButton::clicked, this, &Characterselect::selectMage);
    addWidget(selectMage_Button);

    QPushButton* selectWarrior_Button = new QPushButton();
    selectWarrior_Button->setText("Warrior");
    selectWarrior_Button->setObjectName(QString("selectWarrior_Button"));
    selectWarrior_Button->setToolTip("Warrior");
    selectWarrior_Button->setGeometry(QRect(690, 400, 100, 32)); // puts button at coords 640,360 with width/height 100/32
    selectWarrior_Button->setAutoFillBackground(false);
    connect(selectWarrior_Button, &QPushButton::clicked, this, &Characterselect::selectWarrior);
    addWidget(selectWarrior_Button);
}

void Characterselect::selectMage()
{
    game->openLevel1();
}

void Characterselect::selectWarrior()
{
    game->openLevel1();
}

