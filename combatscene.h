#ifndef COMBATSCENE_H
#define COMBATSCENE_H

#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class Game;

class CombatScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CombatScene(Game* game, QObject* parent = nullptr);
    void initialise();

private:
    Game* game;

    int playerHp;
    int bossHp;
    bool combatOver;

    QLabel* playerHpLabel;
    QLabel* bossHpLabel;
    QLabel* handLabel;
    QLabel* bossActionLabel;
    QLabel* selectedCardLabel;

    QProgressBar* playerHpBar;
    QProgressBar* bossHpBar;

    QPushButton* attackButton;
    QPushButton* healButton;
    QPushButton* blockButton;

    void updateUi();
    void bossAttack();
    bool checkWinLose();
    void handleBossTurn();

private slots:
    void playStrike();
    void playHeal();
    void playBlock();
};

#endif // COMBATSCENE_H
