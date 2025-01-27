#include "Monster.hpp"

Entity* CreateMonster::Slime(int xPos = 0, int yPos = 0)
{
    Entity* slime = &Game::entityManager->addEntity();

    slime->addComponent<StatsComponent>();
    slime->addComponent<PositionComponent>();
    slime->addComponent<HitBoxComponent>();
    slime->addComponent<SpriteRendererComponent>().setSprite("assets/Blob.png");

    slime->getComponent<PositionComponent>().setPos(xPos, yPos);

    return slime;
}