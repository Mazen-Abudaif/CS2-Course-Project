#ifndef COMBATSCENE_H
#define COMBATSCENE_H
#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>

class Game;

class CombatScene: public QGraphicsScene {
    Q_OBJECT

public:
    CombatScene(Game* game, QObject* parent = nullptr);
    void initialise();
private:
    Game* game;
    int playerHp;
    int bossHp;
    bool combatOver;

    void bossAttack();
    bool checkWinLose();
    void updateUi();
    void handleBossTurn();


    QLabel* playerHpLabel;
    QLabel* bossHpLabel;
    QLabel* handLabel;
    QLabel* bossActionLabel;



private slots:
     void playStrike();
     void playHeal();


};





#endif // COMBATSCENE_H
