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

    reward1Button->setFixedSize(140, 180);
    reward2Button->setFixedSize(140, 180);

    selectedRewardLabel = new QLabel("Selected Reward: None");

    addWidget(titleLabel)->setPos(570, 50);
    addWidget(reward1Button)->setPos(430, 320);
    addWidget(reward2Button)->setPos(710, 320);
    addWidget(selectedRewardLabel)->setPos(570, 520);
    addWidget(continueButton)->setPos(570, 540);

    connect(reward1Button, &QPushButton::clicked, this, &RewardScene::chooseReward1);
    connect(reward2Button, &QPushButton::clicked, this, &RewardScene::chooseReward2);
    connect(continueButton, &QPushButton::clicked, this, &RewardScene::continueAfterReward);
}

void RewardScene::chooseReward1()
{
    selectedReward = "Greater Heal";
    selectedRewardLabel->setText("Selected Reward: " + selectedReward);
    selectedRewardLabel->adjustSize();
}

void RewardScene::chooseReward2()
{
    selectedReward = "Dagger";
    selectedRewardLabel->setText("Selected Reward: " + selectedReward);
    selectedRewardLabel->adjustSize();
}

void RewardScene::continueAfterReward()
{
    game->openLevel1(); // Temporary placeholder until the next level is implemented
}