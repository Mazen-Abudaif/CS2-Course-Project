#ifndef COMBATSCENE_H
#define COMBATSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class Game;
class Boss;

class CombatScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CombatScene(Game* game, int bossMaxHp = 60, int immuneTurns = 0, QObject* parent = nullptr);
    void initialise();

private:
    Game* game;

    int playerHp;
    int bossMaxHp;
    Boss* boss;
    bool combatOver;
    bool playerTurn;
    bool playerBlocking;
    int bossImmuneTurns;
    bool abilityUsed;



    QLabel* playerHpLabel;
    QLabel* bossHpLabel;
    QLabel* turnLabel;
    QLabel* bossActionLabel;
    QLabel* selectedCardLabel;

    QProgressBar* playerHpBar;
    QProgressBar* bossHpBar;

    QGraphicsPixmapItem* playerSprite;
    QGraphicsPixmapItem* enemySprite;

    QPushButton* attackButton;
    QPushButton* healButton;
    QPushButton* blockButton;
    QPushButton* abilityButton;

    void updateUi();
    void bossAttack();
    bool checkWinLose();
    void handleBossTurn();
    void applyAttackCard();
    void applyHealCard();
    void applyBlockCard();
    void applyAbility();

private slots:
    void playStrike();
    void playHeal();
    void playBlock();
    void playAbility();
};

#endif // COMBATSCENE_H
