#include "game.h"

#include "upgradeLayout.h"

Game::Game(RenderManager *renderManager, UpdateManager *updateManager,
           CollisionManager *collisionManager, World *world,
           TextureManager *textureManager)
    : world(world),
      projectileManager(*renderManager, *updateManager, *collisionManager),
      shipManager(*renderManager, *updateManager, *collisionManager, *world,
                  *textureManager, projectileManager.getSpawner()),
      waveSpawner(1.1f, 10, shipManager, *world),
      result(GameResult::InProgress),
      radar({700, 7, 1, 0, 0, {}}),
      upgradeManager(3),
      upgradeWindowActive(false)
{
    initPlayerShip();
    startRound();
}

Game::~Game()
{
    projectileManager.clear();
}

void Game::update(float delta)
{
    if (result != GameResult::InProgress)
        return;

    if (upgradeWindowActive)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            chooseUpgrade(GetMousePosition(), {(float)GetScreenWidth(), (float)GetScreenHeight()});
        return;
    }

    shipManager.cleanupDestroyedShips();
    projectileManager.cleanup();

    fillRadar(delta);

    if (shipManager.getPlayerShip()->isDead())
    {
        result = GameResult::Lose;
        return;
    }

    if (shipManager.hasNoEnemyShips())
    {
        if (waveSpawner.hasNextWave())
        {
            upgrade();
        }
        else
        {
            result = GameResult::Win;
        }
    }
}

void Game::startRound()
{
    waveSpawner.spawnNextWave();
}

void Game::resetGame()
{
    projectileManager.clear();
    shipManager.clear();
    waveSpawner.reset();
    result = GameResult::InProgress;
    upgradeWindowActive = false;
    initPlayerShip();
    startRound();
}

// TODO check that its okay this has been changed to return center
GameView Game::getView() const
{
    const PlayerShip *player = shipManager.getPlayerShip();
    const Rectangle playerBody = player->getBody();
    int currentRound;
    int maxRounds;
    waveSpawner.getRoundInfo(currentRound, maxRounds);

    return {
        currentRound,
        maxRounds,
        player->getHealth(),
        player->getMaxHealth(),
        {playerBody.x + playerBody.width / 2.0f,
         playerBody.y + playerBody.height / 2.0f},
        result,
        upgradeWindowActive};
}

void Game::initPlayerShip()
{
    Vector2 area = world->getDimensions();

    ShipData shipData{{area.x / 2, area.y / 2},
                      {40, 40},
                      {64, 64},
                      600.0f,
                      0.0f,
                      100.0f,
                      1.0f};

    shipData.wrapAtEdges = true;
    shipManager.createShip(ShipManager::ShipType::Player, shipData);
}

// fill radar with enemies currently "currentRadius" distance or less from player
void Game::fillRadar(float delta)
{
    radar.updateScanning(delta);
    radar.clearActiveShips();

    Rectangle pBody = shipManager.getPlayerShip()->getBody();
    Vector2 pPosition = {pBody.x + pBody.width / 2.0f,
                         pBody.y + pBody.height / 2.0f};

    auto shipPositions = shipManager.getEnemyPositions();
    int radius = radar.getData().currentRadius;

    for (const auto &enemyPos : shipPositions)
    {
        float dx = enemyPos.x - pPosition.x;
        float dy = enemyPos.y - pPosition.y;

        float distance = sqrtf(dx * dx + dy * dy);

        if (distance <= radius)
        {
            radar.addActiveShip(enemyPos);
        }
    }
}

void Game::upgrade()
{
    upgradeManager.generateChoices();
    upgradeWindowActive = true;
}

void Game::chooseUpgrade(Vector2 mousePosition, Vector2 viewportSize)
{
    if (viewportSize.x <= 0.0f || viewportSize.y <= 0.0f)
        return;

    const Vector2 normalizedMouse = {
        mousePosition.x / viewportSize.x,
        mousePosition.y / viewportSize.y};
    const int choiceCount = static_cast<int>(upgradeManager.getUpgradeTypes().size());

    for (int index = 0; index < choiceCount; ++index)
    {
        const Rectangle panel = UpgradeLayout::panel({1.0f, 1.0f}, index, choiceCount);
        if (CheckCollisionPointRec(normalizedMouse, panel))
        {
            shipManager.getPlayerShip()->applyUpgrade(upgradeManager.getUpgradeTypes()[index]);
            upgradeWindowActive = false;
            startRound();
            return;
        }
    }
}