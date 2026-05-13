#include "combatscene.h"
#include "game.h"
#include "boss.h"

#include <QString>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <cstdlib>


CombatScene::CombatScene(Game* game, int bossMaxHp, int immuneTurns, QObject* parent)
    : QGraphicsScene(parent),
    game(game),
    playerHp(100),
    bossMaxHp(bossMaxHp),
    boss(new Boss(bossMaxHp)),
    combatOver(false),
    playerTurn(true),
    playerBlocking(false),
    bossImmuneTurns(immuneTurns),
    abilityUsed(false),
    playerHpLabel(nullptr),
    bossHpLabel(nullptr),
    turnLabel(nullptr),
    bossActionLabel(nullptr),
    selectedCardLabel(nullptr),
    playerHpBar(nullptr),
    bossHpBar(nullptr),
    attackButton(nullptr),
    healButton(nullptr),
    blockButton(nullptr),
    abilityButton(nullptr),
    playerSprite(nullptr),
    enemySprite(nullptr)
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

    // player sprite — left side of the scene
    QPixmap playerPixmap(":/images/Images/skin.png") ;
    playerPixmap = playerPixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation) ;
    playerSprite = new QGraphicsPixmapItem(playerPixmap) ;
    playerSprite->setPos(150, 280) ;
    playerSprite->setZValue(1) ;
    addItem(playerSprite) ;

    // enemy sprite — right side of the scene
    QPixmap enemyPixmap(":/images/Images/demogorgon (enemy).png") ;
    enemyPixmap = enemyPixmap.scaled(180, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation) ;
    enemySprite = new QGraphicsPixmapItem(enemyPixmap) ;
    enemySprite->setPos(950, 260) ;
    enemySprite->setZValue(1) ;
    addItem(enemySprite) ;

    playerHpLabel = new QLabel("Player HP");
    bossHpLabel = new QLabel("Boss HP");
    turnLabel = new QLabel("Turn: Player");
    bossActionLabel = new QLabel("Boss Action: None");
    selectedCardLabel = new QLabel("Selected Card: None");

    playerHpBar = new QProgressBar();
    playerHpBar->setRange(0, 100);
    playerHpBar->setValue(playerHp);
    playerHpBar->setFormat("%v / %m");

    bossHpBar = new QProgressBar();
    bossHpBar->setRange(0, bossMaxHp);
    bossHpBar->setValue(boss->getHealth());
    bossHpBar->setFormat("%v / %m");

    attackButton = new QPushButton("Attack Card");
    healButton = new QPushButton("Heal Card");
    blockButton = new QPushButton("Block Card");

    attackButton->setFixedSize(140, 180);
    healButton->setFixedSize(140, 180);
    blockButton->setFixedSize(140, 180);

    QString character = game->getSelectedCharacter();

    if (character == "Mage")
        abilityButton = new QPushButton("Fireball\n(20 dmg, once)");
    else if (character == "Warrior")
        abilityButton = new QPushButton("Shield Bash\n(8 dmg + block)");

    addWidget(playerHpLabel)->setPos(50, 40);
    addWidget(playerHpBar)->setPos(50, 70);

    addWidget(bossHpLabel)->setPos(900, 40);
    addWidget(bossHpBar)->setPos(900, 70);

    addWidget(bossActionLabel)->setPos(500, 140);
    addWidget(selectedCardLabel)->setPos(500, 180);

    addWidget(turnLabel)->setPos(560, 110);

    addWidget(attackButton)->setPos(360, 520);
    addWidget(healButton)->setPos(560, 520);
    addWidget(blockButton)->setPos(760, 520);

    if (abilityButton != nullptr)
    {
        abilityButton->setFixedSize(140, 180);
        addWidget(abilityButton)->setPos(960, 520);
        connect(abilityButton, &QPushButton::clicked, this, &CombatScene::playAbility);
    }

    connect(attackButton, &QPushButton::clicked, this, &CombatScene::playStrike);
    connect(healButton, &QPushButton::clicked, this, &CombatScene::playHeal);
    connect(blockButton, &QPushButton::clicked, this, &CombatScene::playBlock);

    updateUi();
}

void CombatScene::updateUi()
{
    playerHpBar->setValue(playerHp);
    bossHpBar->setValue(boss->getHealth());

    playerHpLabel->setText("Player HP");
    bossHpLabel->setText("Boss HP");

    if (playerTurn)
        turnLabel->setText("Turn: Player");
    else
        turnLabel->setText("Turn: Boss");



    playerHpLabel->adjustSize();
    bossHpLabel->adjustSize();
    bossActionLabel->adjustSize();
    selectedCardLabel->adjustSize();
    turnLabel->adjustSize();

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
    if (abilityButton != nullptr)
        abilityButton->setEnabled(false);

    QTimer::singleShot(1000, this, [this]() {
        if (combatOver)
            return;

        bossAttack();
        playerTurn = true;
        updateUi();
        checkWinLose();

        if (!combatOver)
        {
            attackButton->setEnabled(true);
            healButton->setEnabled(true);
            blockButton->setEnabled(true);
            if (abilityButton != nullptr && !abilityUsed)
                abilityButton->setEnabled(true);
        }
    });
}

void CombatScene::bossAttack()
{
    if (bossImmuneTurns > 0)
    {
        QString turnsText = (bossImmuneTurns == 1) ? "1 turn left" : QString::number(bossImmuneTurns) + " turns left";
        bossActionLabel->setText("Boss is immune! (" + turnsText + ")");
        bossImmuneTurns--;
        return;
    }

    int action = rand() % 2;
    int damage;

    if (action == 0)
    {
        bossActionLabel->setText("Boss Action: Claw");
        damage = 8;
    }
    else
    {
        bossActionLabel->setText("Boss Action: Heavy Strike");
        damage = 12;
    }

    if (playerBlocking)
    {
        damage /= 2;
        playerBlocking = false;
    }

    playerHp -= damage;
    if (playerHp < 0)
        playerHp = 0;
}

bool CombatScene::checkWinLose()
{
    if (boss->getHealth() <= 0) {
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
    if (bossImmuneTurns > 0)
    {
        bossActionLabel->setText("Boss is immune — attack had no effect!");
        return;
    }
    boss->decreaseHealth(10);
}

void CombatScene::applyBlockCard()
{
    playerBlocking = true;
    bossActionLabel->setText("Boss Action: Blocked — next hit halved");
}
void CombatScene::applyHealCard()
{
    playerHp += 10;
    if (playerHp > 100)
        playerHp = 100;
}

void CombatScene::playAbility()
{
    if (combatOver || !playerTurn || abilityUsed)
        return;

    selectedCardLabel->setText("Selected Card: Ability");
    applyAbility();

    if (!abilityUsed)  // ability was blocked by immunity, don't end turn
        return;

    playerTurn = false;
    updateUi();

    if (checkWinLose())
        return;

    handleBossTurn();
}

void CombatScene::applyAbility()
{
    QString character = game->getSelectedCharacter();

    if (character == "Mage")
    {
        if (bossImmuneTurns > 0)
        {
            bossActionLabel->setText("Boss is immune — Fireball had no effect!");
            return;
        }
        boss->decreaseHealth(20);
        abilityUsed = true;
        abilityButton->setEnabled(false);
    }
    else if (character == "Warrior")
    {
        if (bossImmuneTurns > 0)
        {
            bossActionLabel->setText("Boss is immune — Shield Bash had no effect!");
            return;
        }
        boss->decreaseHealth(8);
        playerBlocking = true;
        abilityUsed = true;
        abilityButton->setEnabled(false);
    }
}
