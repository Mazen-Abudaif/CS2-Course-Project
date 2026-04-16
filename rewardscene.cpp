#include "rewardscene.h"
#include "game.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsPixmapItem>

RewardScene::RewardScene(Game* game, QObject* parent)
    : QGraphicsScene(parent),
    game(game),
    selectedReward(""),
    selectedRewardLabel(nullptr)
{
}
// Sets up the reward selection screen
void RewardScene::initialise()
{

    setSceneRect(0, 0, 1280, 720);

    QPixmap background(":/images/Images/reward_bg_1280x720.png");
    background = background.scaled(1280, 720, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QGraphicsPixmapItem* backgroundItem = new QGraphicsPixmapItem(background);
    addItem(backgroundItem);
    backgroundItem->setZValue(-1);

    QLabel* titleLabel = new QLabel("Choose a Reward");
    QPushButton* reward1Button = new QPushButton("Boss Card 1");
    QPushButton* reward2Button = new QPushButton("Boss Card 2");
    QPushButton* continueButton = new QPushButton("Continue");

    selectedRewardLabel = new QLabel("Selected Reward: None");

    addWidget(titleLabel)->setPos(50, 50);
    addWidget(reward1Button)->setPos(50, 120);
    addWidget(reward2Button)->setPos(50, 170);
    addWidget(selectedRewardLabel)->setPos(50, 240);
    addWidget(continueButton)->setPos(50, 310);

    connect(reward1Button, &QPushButton::clicked, this, &RewardScene::chooseReward1);
    connect(reward2Button, &QPushButton::clicked, this, &RewardScene::chooseReward2);
    connect(continueButton, &QPushButton::clicked, this, &RewardScene::continueAfterReward);
}

void RewardScene::chooseReward1()
{
    selectedReward = "Boss Card 1";
    selectedRewardLabel->setText("Selected Reward: " + selectedReward);
    selectedRewardLabel->adjustSize();
}

void RewardScene::chooseReward2()
{
    selectedReward = "Boss Card 2";
    selectedRewardLabel->setText("Selected Reward: " + selectedReward);
    selectedRewardLabel->adjustSize();
}

void RewardScene::continueAfterReward()
{
    game->openLevel1(); // Temporary placeholder until the next level is implemented
}