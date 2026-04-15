#ifndef REWARDSCENE_H
#define REWARDSCENE_H

#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>
#include <QString>

class Game;

class RewardScene : public QGraphicsScene
{
    Q_OBJECT

public:
    RewardScene(Game* game, QObject* parent = nullptr);
    void initialise();

private:
    Game* game;
    QString selectedReward;
    QLabel* selectedRewardLabel;

private slots:
    void chooseReward1();
    void chooseReward2();
    void continueAfterReward();
};

#endif // REWARDSCENE_H
