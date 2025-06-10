#include "CreateMonster.hpp"
#include "Components.hpp"
#include "Game.hpp"
#include <iostream>

Entity* CreateMonster::Slime(Manager* entityManager, Entity* tileMap, int xPos, int yPos)
{
    Entity* slime = &entityManager->addEntity();
    
    slime->addComponent<StatsComponent>();
    slime->addComponent<PositionComponent>();
    slime->addComponent<MouvementComponent>();
    slime->addComponent<PathFinderComponent>().setTileMap(tileMap);
    slime->addComponent<HitBoxComponent>().addHitBox(HitBox(0, 0, 64, 64));
    slime->addComponent<SpriteRendererComponent>().setSprite("assets/Blob.png");
    slime->getComponent<PositionComponent>().setPos(xPos, yPos);
    slime->getComponent<StatsComponent>().;

    auto animator = &slime->addComponent<AnimatorComponent>();
    animator->loadFolder("assets/EntitySprite/Slime/");
    animator->setCurrentSpriteSheet("Idle");    
    animator->playAnimationIn("Jump", 0.5);
    animator->playAnimationIn("Attack", 0.75);

    auto slimeAI = &slime->addComponent<SlimeAI>();
    slimeAI->setTileMap(tileMap);

    animator->addConditions("Idle", []()
    {
        return true;
    });

     animator->addConditions("Jump", [slimeAI]()
    {
        if(slimeAI->getIsJumping()) return true;
        return false;
    });

     animator->addConditions("Attack", [slimeAI]()
    {
        if(slimeAI->getIsAttacking()) return true;
        return false;
    });

    
    return slime;
}
