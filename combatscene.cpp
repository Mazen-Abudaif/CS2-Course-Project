#include "combatscene.h"
#include "game.h"
#include <QString>
#include <QGraphicsProxyWidget>
#include <QMessageBox>

// initializes combat values
CombatScene::CombatScene(Game* game, QObject* parent)
    : QGraphicsScene(parent), game(game), playerHp(100), bossHp(60), combatOver(false),
    playerHpLabel(nullptr), bossHpLabel(nullptr) {}

// sets UI for combat
void CombatScene::initialise() {
    setSceneRect(0, 0, 1280, 720);

    playerHpLabel = new QLabel("Player HP: 100");
    bossHpLabel = new QLabel("Boss HP: 60");

    QPushButton* strikeButton = new QPushButton("Strike");
    QPushButton* healButton = new QPushButton("Heal");


    addWidget(playerHpLabel)->setPos(50, 50);
    addWidget(bossHpLabel)->setPos(50, 100);
    addWidget(strikeButton)->setPos(50, 180);
    addWidget(healButton)->setPos(50, 230);

    //connects buttons to action
    connect(strikeButton, &QPushButton::clicked, this, &CombatScene::playStrike);
    connect(healButton, &QPushButton::clicked, this, &CombatScene::playHeal);

}
void CombatScene::updateUi() {
    playerHpLabel->setText("Player HP: " + QString::number(playerHp));
    bossHpLabel->setText("Boss HP: " + QString::number(bossHp));

}
void CombatScene::playStrike() {
    if (combatOver) // stops any actions after combat has ended
        return;
    bossHp-=10;
    if ( bossHp < 0)
        bossHp = 0;

    updateUi();

    if (checkWinLose())
        return;

    bossAttack();

    checkWinLose();
}
void CombatScene::playHeal() {
    if (combatOver)
        return;
    playerHp+=10;
    if (playerHp > 100 )
        playerHp = 100;

    updateUi();

    if (checkWinLose())
        return;

    bossAttack();

    checkWinLose();
}
void CombatScene::bossAttack() {
    playerHp-=8;
    if (playerHp < 0)
        playerHp = 0;

    updateUi();
}
// checks whether combat has ended
bool CombatScene::checkWinLose() {
    if( bossHp <= 0 ) {
        combatOver = true;
        QMessageBox::information(nullptr, "Combat", "You Win!!");
        game->openReward(); // player is directed to reward scence
        return true;
    }

    if (playerHp <= 0 ) {
        combatOver = true;
        QMessageBox::information(nullptr, "Combat", "You Lose :(");
        game->openLevel1(); // player retries level
        return true;
    }
    return false;
}
