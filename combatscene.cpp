#include "combatscene.h"
#include "game.h"

#include <QString>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include <QGraphicsPixmapItem>

CombatScene::CombatScene(Game* game, QObject* parent)
    : QGraphicsScene(parent),
    game(game),
    playerHp(100),
    bossHp(60),
    combatOver(false),
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
    handLabel = new QLabel("Your Hand");
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

    addWidget(handLabel)->setPos(530, 470);

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

    playerHpLabel->adjustSize();
    bossHpLabel->adjustSize();
    bossActionLabel->adjustSize();
    selectedCardLabel->adjustSize();
}

void CombatScene::playStrike()
{
    if (combatOver)
        return;

    selectedCardLabel->setText("Selected Card: Attack Card");

    bossHp -= 10;
    if (bossHp < 0)
        bossHp = 0;

    updateUi();

    if (checkWinLose())
        return;

    handleBossTurn();
}

void CombatScene::playHeal()
{
    if (combatOver)
        return;

    selectedCardLabel->setText("Selected Card: Heal Card");

    playerHp += 10;
    if (playerHp > 100)
        playerHp = 100;

    updateUi();

    if (checkWinLose())
        return;

    handleBossTurn();
}

void CombatScene::playBlock()
{
    if (combatOver)
        return;

    selectedCardLabel->setText("Selected Card: Block Card");
    bossActionLabel->setText("Boss Action: Block reduced damage");

    playerHp -= 3;
    if (playerHp < 0)
        playerHp = 0;

    updateUi();

    checkWinLose();
}

void CombatScene::handleBossTurn()
{
    bossAttack();
    checkWinLose();
}

void CombatScene::bossAttack()
{
    bossActionLabel->setText("Boss Action: Claw");

    playerHp -= 8;
    if (playerHp < 0)
        playerHp = 0;

    updateUi();
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