#include "rewardscene.h"
#include "game.h"
#include "player.h"
#include "cardtype.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsPixmapItem>
#include <QSettings>

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

    QString sc ;

    if(game->current_level==5)
    {
        sc = ":/images/Images/final_reward.png" ;
    }
    else
    {
        sc = ":/images/Images/reward_bg_1280x720.png" ;
    }
    QPixmap background(sc);
    background = background.scaled(1280, 720, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QGraphicsPixmapItem* backgroundItem = new QGraphicsPixmapItem(background);
    addItem(backgroundItem);
    backgroundItem->setZValue(-1);

    if(game->current_level!=5) {
    QLabel* titleLabel = new QLabel("Choose a Reward");
    QPushButton* reward1Button = new QPushButton("Greater Heal\n(Heal +20)");
    QPushButton* reward2Button = new QPushButton("Dagger\n(Attack +5)");

    reward1Button->setFixedSize(140, 180);
    reward2Button->setFixedSize(140, 180);

    selectedRewardLabel = new QLabel("Selected Reward: None");

    addWidget(titleLabel)->setPos(570, 50);
    addWidget(reward1Button)->setPos(430, 320);
    addWidget(reward2Button)->setPos(710, 320);
    addWidget(selectedRewardLabel)->setPos(570, 520);

    connect(reward1Button, &QPushButton::clicked, this, &RewardScene::chooseReward1);
    connect(reward2Button, &QPushButton::clicked, this, &RewardScene::chooseReward2); }
    QPushButton* continueButton = new QPushButton("Continue");
    addWidget(continueButton)->setPos(570, 540);
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
    game->setRewardCard(selectedReward);



    Player* player = game->getPlayer();
    if (player)
    {
        game->openNextLevel();
        QSettings settings("MyGame", "SaveData");
        settings.setValue("level", game->current_level+1);
        settings.setValue("character", game->getSelectedCharacter());
        settings.setValue("rewardCard", selectedReward);
        settings.setValue("hp", player->getHealth());

        QStringList cardList;
        for (Card* card : player->deck)
        {
            if (card->getType() == CardType::Attack)
                cardList.append("Attack:" + QString::number(card->getDamage()));
            else if (card->getType() == CardType::Block)
                cardList.append("Block:" + QString::number(card->getDamage()));
            else if (card->getType() == CardType::Heal)
                cardList.append("Heal:" + QString::number(card->getDamage()));
        }
        settings.setValue("deck", cardList);
    }


}