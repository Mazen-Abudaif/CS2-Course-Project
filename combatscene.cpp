#include "combatscene.h"
#include "game.h"
#include <QString>
#include <QGraphicsProxyWidget>
#include <QMessageBox>

CombatScene::CombatScene(Game* game, QObject* parent)
    : QGraphicsScene(parent),
    game(game),
    playerHp(100),
    bossHp(60),
    combatOver(false),
    playerHpLabel(nullptr),
    bossHpLabel(nullptr),
    handLabel(nullptr),
    bossActionLabel(nullptr)

{
}

// Sets up the combat UI
void CombatScene::initialise()
{
    setSceneRect(0, 0, 1280, 720);

    playerHpLabel = new QLabel("Player HP: 100");
    bossHpLabel = new QLabel("Boss HP: 60");
    handLabel = new QLabel("Your Hand");
    bossActionLabel = new QLabel("Boss Action: None");

    QPushButton* attackButton = new QPushButton("Attack Card");
    QPushButton* healButton = new QPushButton("Heal Card");

    addWidget(playerHpLabel)->setPos(50, 50);
    addWidget(bossHpLabel)->setPos(50, 100);
    addWidget(bossActionLabel)->setPos(50, 150);
    addWidget(handLabel)->setPos(50, 230);

    addWidget(attackButton)->setPos(50, 280);
    addWidget(healButton)->setPos(200, 280);

    // Connects button clicks to player actions
    connect(attackButton, &QPushButton::clicked, this, &CombatScene::playStrike);
    connect(healButton, &QPushButton::clicked, this, &CombatScene::playHeal);
}

void CombatScene::updateUi()
{
    playerHpLabel->setText("Player HP: " + QString::number(playerHp));
    bossHpLabel->setText("Boss HP: " + QString::number(bossHp));

}

void CombatScene::playStrike() {
    if (combatOver)
        return;

    bossHp -= 10;
    if (bossHp < 0)
        bossHp = 0;

    updateUi();

    if (checkWinLose())
        return;

    handleBossTurn();
}

void CombatScene::playHeal() {
    if (combatOver)
        return;

    playerHp += 10;
    if (playerHp > 100)
        playerHp = 100;

    updateUi();

    handleBossTurn();
}

// Simple boss attack
void CombatScene::bossAttack() {
    bossActionLabel->setText("Boss Action: Claw");
    bossActionLabel->adjustSize();

    playerHp -= 8;
    if (playerHp < 0)
        playerHp = 0;

    updateUi();
}

// Checks whether combat has ended
bool CombatScene::checkWinLose() {
    if (bossHp <= 0) {
        combatOver = true;
        QMessageBox::information(nullptr, "Combat", "You Win!");
        game->openReward();
        return true;
    }

    if (playerHp <= 0) {
        combatOver = true;
        QMessageBox::information(nullptr, "Combat", "You Lose!");
        game->openLevel1();
        return true;
    }

    return false;
}
void CombatScene::handleBossTurn() {
    bossAttack();
    checkWinLose();
}