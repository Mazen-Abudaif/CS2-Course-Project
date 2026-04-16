#include "combatscene.h"
#include "game.h"

#include <QString>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <cstdlib>


CombatScene::CombatScene(Game* game, QObject* parent)
    : QGraphicsScene(parent),
    game(game),
    playerHp(100),
    bossHp(60),
    combatOver(false),
    playerTurn(true),
    playerHpLabel(nullptr),
    bossHpLabel(nullptr),
    handLabel(nullptr),
    bossActionLabel(nullptr),
    selectedCardLabel(nullptr),
    playerHpBar(nullptr),
    bossHpBar(nullptr),
    attackButton(nullptr),
    healButton(nullptr),
    blockButton(nullptr)
{
}

void CombatScene::initialise()
{
    setSceneRect(0, 0, 1280, 720);

    QPixmap background(":/images/Images/combat_bg_1280x720.png");
    background = background.scaled(1280, 720, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QGraphicsPixmapItem* backgroundItem = new QGraphicsPixmapItem(background);
    addItem(backgroundItem);
    backgroundItem->setZValue(-1);

    playerHpLabel = new QLabel("Player HP");
    bossHpLabel = new QLabel("Boss HP");
    handLabel = new QLabel("Turn: Player");
    bossActionLabel = new QLabel("Boss Action: None");
    selectedCardLabel = new QLabel("Selected Card: None");

    playerHpBar = new QProgressBar();
    playerHpBar->setRange(0, 100);
    playerHpBar->setValue(playerHp);
    playerHpBar->setFormat("%v / %m");

    bossHpBar = new QProgressBar();
    bossHpBar->setRange(0, 60);
    bossHpBar->setValue(bossHp);
    bossHpBar->setFormat("%v / %m");

    attackButton = new QPushButton("Attack Card");
    healButton = new QPushButton("Heal Card");
    blockButton = new QPushButton("Block Card");

    attackButton->setFixedSize(140, 180);
    healButton->setFixedSize(140, 180);
    blockButton->setFixedSize(140, 180);

    addWidget(playerHpLabel)->setPos(50, 40);
    addWidget(playerHpBar)->setPos(50, 70);

    addWidget(bossHpLabel)->setPos(900, 40);
    addWidget(bossHpBar)->setPos(900, 70);

    addWidget(bossActionLabel)->setPos(500, 140);
    addWidget(selectedCardLabel)->setPos(500, 180);

    addWidget(handLabel)->setPos(560, 110);

    addWidget(attackButton)->setPos(360, 520);
    addWidget(healButton)->setPos(560, 520);
    addWidget(blockButton)->setPos(760, 520);

    connect(attackButton, &QPushButton::clicked, this, &CombatScene::playStrike);
    connect(healButton, &QPushButton::clicked, this, &CombatScene::playHeal);
    connect(blockButton, &QPushButton::clicked, this, &CombatScene::playBlock);

    updateUi();
}

void CombatScene::updateUi()
{
    playerHpBar->setValue(playerHp);
    bossHpBar->setValue(bossHp);

    playerHpLabel->setText("Player HP");
    bossHpLabel->setText("Boss HP");

    if (playerTurn)
        handLabel->setText("Turn: Player");
    else
        handLabel->setText("Turn: Boss");



    playerHpLabel->adjustSize();
    bossHpLabel->adjustSize();
    bossActionLabel->adjustSize();
    selectedCardLabel->adjustSize();
    handLabel->adjustSize();

}

void CombatScene::playStrike()
{
    if (combatOver || !playerTurn)
        return;

    selectedCardLabel->setText("Selected Card: Attack Card");
    applyAttackCard();

    playerTurn = false;

    updateUi();

    if (checkWinLose())
        return;

    handleBossTurn();
}

void CombatScene::playHeal()
{
    if (combatOver || !playerTurn)
        return;

    selectedCardLabel->setText("Selected Card: Heal Card");
    applyHealCard();


    playerTurn = false;

    updateUi();

    if (checkWinLose())
        return;

    handleBossTurn();
}

void CombatScene::playBlock()
{
    if (combatOver || !playerTurn )
        return;

    selectedCardLabel->setText("Selected Card: Block Card");
    applyBlockCard();

    playerTurn = false;

    updateUi();

    if (checkWinLose())
        return;

    handleBossTurn();
}

void CombatScene::handleBossTurn()
{
    updateUi();

    attackButton->setEnabled(false);
    healButton->setEnabled(false);
    blockButton->setEnabled(false);

    QTimer::singleShot(1000, this, [this]() {
        if (combatOver)
            return;

    bossAttack();
    playerTurn = true;
    updateUi();
    checkWinLose();

    if (!combatOver) {
        attackButton->setEnabled(true);
        healButton->setEnabled(true);
        blockButton->setEnabled(true);
    }

    });
}

void CombatScene::bossAttack()
{
    int action = rand() % 2;

    if (action == 0) {
        bossActionLabel->setText("Boss Action: Claw");
        playerHp -= 8;
    } else {
        bossActionLabel->setText("Boss Action: Heavy Strike");
        playerHp -= 12;
    }

    if (playerHp < 0)
        playerHp = 0;
}

bool CombatScene::checkWinLose()
{
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
void CombatScene::applyAttackCard()
{
    bossHp -= 10;
    if (bossHp < 0)
        bossHp = 0;
}

void CombatScene::applyBlockCard()
{
    bossActionLabel->setText("Boss Action: Block reduced damage");
}
void CombatScene::applyHealCard()
{
    playerHp += 10;
    if (playerHp > 100)
        playerHp = 100;
}
